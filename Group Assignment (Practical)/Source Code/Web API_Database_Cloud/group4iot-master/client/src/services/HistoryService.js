export default {
  async search(store, axios, query) {
    const qr = []
    qr.push(`page=${store.state.page}&limit=${store.state.limit}`)
    if (query.fromDate && query.toDate) {
      qr.push(`fromDate=${query.fromDate}`)
      qr.push(`toDate=${query.toDate}`)
    }
    const res = await axios.get(`/history/${qr.length > 0 ? `?${qr.join('&')}` : ''}`)
    return res.data
  },
}
