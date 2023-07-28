import express from 'express'
import historyRepo from '../model/history.js'

const routers = express.Router()
routers.get('/', async (req, res) => {
  res.render('index.html')
})
routers.get('/health', (req, res) => {
  res.send('Server is running!')
})
routers.get('/history', async (req, res, next) => {
  try {
    const { page, limit, fromDate, toDate } = req.query
    const currentPage = Number(page)
    const limitPerPage = Number(limit)
    let params = {}
    if (fromDate !== undefined && toDate !== undefined) {
      params = {
        dateModified: {
          $gte: fromDate,
          $lt: toDate,
        },
      }
    }
    const histories = await historyRepo
      .find(params)
      .sort({ dateModified: 'desc' })
      .skip((currentPage - 1) * limitPerPage)
      .limit(limitPerPage)
    const totalItem = await historyRepo.countDocuments({}).exec()
    const totalPage = Math.ceil(totalItem / limitPerPage)
    res.status(200).send({ code: 200, success: true, data: histories, currentPage, totalPage, totalItem })
  } catch (err) {
    next(err)
  }
})
export default routers
