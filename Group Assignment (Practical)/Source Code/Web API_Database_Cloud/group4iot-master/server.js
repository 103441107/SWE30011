import cors from 'cors'
import * as path from 'path'
import * as http from 'http'
import dotenv from 'dotenv'
import express from 'express'
import Server from 'socket.io'
import mongoose from 'mongoose'
import bodyParser from 'body-parser'
import errorHandler from './middleware/error.js'
import history from 'connect-history-api-fallback'
import routers from './routes/index.js'
import historyRepo from './model/history.js'

const app = express()
const server = http.Server(app)
const io = new Server(server)
const __dirname = path.resolve()
dotenv.config()

app.use(history())
app.use(bodyParser.json())
app.use(cors({ origin: '*' }))
app.use(bodyParser.urlencoded({ extended: true }))
app.use(express.static(path.join(__dirname, 'client/dist')))
app.use('/', routers)
app.use(errorHandler)
let device = {
  id: null,
  isOnline: false,
}
const dbUrl = process.env.DB_URL
mongoose.connect(dbUrl, { useNewUrlParser: true }, (err) => {
  if (err) {
    console.error('Connect to DB error:' + err)
  } else {
    console.log('Connected to DB')
  }
})

io.use(async (socket, next) => {
  if (!socket.handshake.headers['user-agent'].includes('arduino')) {
    next()
  } else {
    socket.espIsOnline = true
    device.id = socket.id
    device.isOnline = true
    next()
  }
})

io.on('connection', (socket) => {
  try {
    if (socket.espIsOnline) {
      io.emit('esp-online')
    }
    if (!socket.handshake.headers['user-agent'].includes('arduino') && device.isOnline) {
      io.emit('esp-online')
    }
  } catch (error) {
    console.log(error)
  }

  socket.on('esp-execute-state', (data) => {
    socket.to(device.id).emit('esp-execute-state', data)
  })

  socket.on('esp-update-state', async (data) => {
    historyRepo.create({
      dateModified: Date.now(),
      state: data.state,
      device: data.id,
    })
    io.emit('esp-update-state', data)
  })
  socket.on('disconnect', () => {
    if (device.id === socket.id) {
      io.emit('esp-offline')
      device.isOnline = false
    }
  })
})

console.log(`Server listen port ${process.env.PORT}`)
server.listen(process.env.PORT || 80)
