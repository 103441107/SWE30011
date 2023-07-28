import { createStore } from 'vuex'
export default createStore({
  state: {
    isLoading: false,
    notification: null,
    page: 1,
    limit: 10,
  },
  mutations: {
    setLoading: (state, isLoading) => (state.isLoading = isLoading),
    setPage: (state, page) => (state.page = page),
    setLimit: (state, limit) => (state.limit = limit),
    setNotification: (state, notification) => (state.notification = notification),
  },
  actions: {
    startLoading({ commit }) {
      commit('setLoading', true)
    },
    stopLoading({ commit }) {
      commit('setLoading', false)
    },
    handleNotifications({ commit }, data) {
      let notification = ''
      if (typeof data.message === 'string') {
        notification = data.message
      }
      commit('setNotification', { text: notification, type: data.success ? 'success' : 'error' })
    },
  },
  modules: {},
})
