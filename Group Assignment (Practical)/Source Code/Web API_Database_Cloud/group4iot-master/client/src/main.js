import { createApp } from 'vue'
import App from './App.vue'
import router from './router'
import './assets/main.css'
import store from './store/index'
import axios from './plugins/axios'
import Notifications from '@kyvg/vue3-notification'
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'
import { library } from '@fortawesome/fontawesome-svg-core'
import { faLightbulb, faUsers, faMicrochip, faPlus, faTrash, faPen, faSearch, faDownload } from '@fortawesome/free-solid-svg-icons'
import DashboardLayout from './layouts/DashboardLayout.vue'
import EmptyLayout from './layouts/EmptyLayout.vue'
const app = createApp(App)
library.add(faLightbulb, faUsers, faMicrochip, faPlus, faTrash, faPen, faSearch, faDownload)
app.config.globalProperties.$axios = axios
app.use(store)
app.use(router)
app.use(Notifications)
app.component('font-awesome-icon', FontAwesomeIcon)
app.component('default-layout', DashboardLayout)
app.component('empty-layout', EmptyLayout)
app.mount('#app')
