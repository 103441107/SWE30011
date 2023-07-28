import mongoose from 'mongoose'
const Schema = mongoose.Schema
const historySchema = new Schema({
  dateModified: Date,
  state: Boolean,
  device: String,
})
export default mongoose.model('history', historySchema, 'histories')
