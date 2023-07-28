<template>
  <div>
    <h3 class="text-2xl font-medium text-gray-700 text-center">Dashboard</h3>

    <div class="">
      <div class="flex flex-wrap -mx-6">
        <div class="w-full px-6 mt-6 sm:w-2/5 xl:w-2/5">
          <div
            class="w-full bg-white border-t-4 rounded-md px-5 py-5 pb-5 pt-3"
            :class="[{ 'border-t-green-500': device.isOnline }, { 'border-t-red-500': !device.isOnline }]">
            <!-- <h5 class="text-gray-900 text-xl leading-tight font-medium mb-3 text-center">Iot Control</h5> -->
            <h3 class="font-semibold text-gray-700 pb-6">Iot Control</h3>
            <div class="w-full inline-grid grid-cols-2 gap-3">
              <div class="w-full block p-6 rounded-lg bg-white border border-slate-300" v-for="sw in device.sw" :key="sw.id">
                <h5 class="text-gray-900 text-l leading-tight font-medium mb-4">{{ sw.name }}</h5>
                <div class="w-full">
                  <span
                    class="cursor-pointer inline-flex items-center justify-center px-2 py-1 text-xs font-medium leading-none text-white rounded mb-4"
                    :class="[{ 'bg-green-500': sw.state }, { 'bg-gray-500': !sw.state }]"
                    >{{ `State ${sw.state ? 'On' : 'Off'}` }}</span
                  >
                </div>
                <button
                  type="button"
                  class="
                    inline-block
                    px-6
                    py-2.5
                    bg-blue-600
                    text-white
                    font-medium
                    text-xs
                    leading-tight
                    rounded
                    shadow-md
                    hover:bg-blue-700 hover:shadow-lg
                    focus:bg-blue-700 focus:shadow-lg focus:outline-none focus:ring-0
                    active:bg-blue-800 active:shadow-lg
                    transition
                    duration-150
                    ease-in-out
                  "
                  @click="
                    () => {
                      toggle(sw.id, !sw.state)
                    }
                  ">
                  Toggle
                </button>
              </div>
            </div>
          </div>
        </div>
        <div class="w-full mt-6 px-6 sm:w-3/5 xl:w-3/5 max-h-full">
          <history-box></history-box>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { defineComponent } from 'vue'
import io from 'socket.io-client'
import HistoryBox from '../components/HistoryBox.vue'

export default defineComponent({
  name: 'Dashboard',
  components: { HistoryBox },
  data: () => {
    return {
      device: {
        isOnline: false,
        sw: [
          { id: 'sw_01', name: 'Switch 01', state: false },
          { id: 'sw_02', name: 'Switch 02', state: false },
        ],
      },
      socket: null,
      url: '',
      isConnected: null,
    }
  },
  async mounted() {
    this.$store.dispatch('startLoading')
    try {
      if (import.meta.env.VITE_NODE_ENV?.toString() === 'development') {
        this.url = import.meta.env.VITE_API_URL?.toString()
      }

      this.socket = io(this.url)
      this.isConnected = true
      this.socket.on('connect_error', () => {
        if (this.isConnected !== false) {
          this.$store.dispatch('handleNotifications', { message: 'Cannot connect to socket server.' })
          this.isConnected = false
        }
      })
      this.socket.on('disconnect', () => {
        this.$store.dispatch('handleNotifications', { message: 'Disconnected from socket server.' })
        this.isConnected = false
      })
      this.socket.on('hello', (data) => {
        console.log(data)
      })
      this.socket.on('esp-update-state', (obj) => {
        const sw = this.device.sw.find((el) => el.id === obj.id)
        sw.state = obj.state
      })

      this.socket.on('esp-online', () => {
        this.device.isOnline = true
      })

      this.socket.on('esp-offline', () => {
        this.device.isOnline = false
      })

      this.socket.on('all-esp-online', (data) => {
        console.log(data)
        data.forEach((el) => {
          const device = this.devices.find((element) => element.espId === el.id)
          device.state = true
        })
      })

      this.socket.on('esp-execute-state', (data) => {
        console.log(data)
      })
    } catch (e) {
      this.isConnected = false
      this.$store.dispatch('handleNotifications', { message: 'Cannot connect to server.' })
    }
    this.$store.dispatch('stopLoading')
  },
  methods: {
    async toggle(id, state) {
      if (!this.isConnected) return
      this.socket.emit('esp-execute-state', { id, state })
    },
    async request(nodeid, switchid) {
      if (!this.isConnected) return
      this.socket.emit('esp-require-sensor', {
        nodeid,
        switchid,
        state: true,
      })
    },
  },
  watch: {
    'device.isOnline': async function (value) {
      const msg = value ? 'Device is online' : 'Device is offline'
      const success = value
      this.$store.dispatch('handleNotifications', { message: msg, success })
    },
  },
})
</script>