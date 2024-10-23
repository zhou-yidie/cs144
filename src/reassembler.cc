#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.
  uint64_t capacity = output_.writer().available_capacity();
  uint64_t left_bound = max(first_index,current_index_);
  uint64_t right_bound = min(current_index_ + capacity, first_index + data.size());
  if (right_bound < left_bound) {
    return;
  }
  reassembler_item item = reassembler_item(
    data.substr(left_bound - first_index, right_bound - left_bound),
    left_bound, right_bound, is_last_substring && right_bound == first_index+data.size()
  );
  pending_size_ += item.data.size();
  auto insert_iter = lower_bound(buffer_.begin(), buffer_.end(), item);
  auto iter = insert_iter;
  while (iter != buffer_.end() && item.last_index >= iter->first_index) {
    if (item.last_index < iter->last_index) {
      item.data += iter->data.substr(item.last_index - iter->first_index);
      pending_size_ -= (item.last_index - iter->first_index);
      item.last_index = iter->last_index;
      item.is_last |= iter->is_last;
    } else {
      pending_size_ -= iter->data.size();
    }
    iter = buffer_.erase(iter);
  }

  if (insert_iter != buffer_.begin()) {
    iter = insert_iter - 1;
    if (iter->last_index >= item.first_index) {
      if (iter->last_index < item.last_index) {
        iter->data += item.data.substr(iter->last_index - item.first_index);
        pending_size_ -= iter->last_index - item.first_index;
        iter->last_index = item.last_index;
        iter->is_last |= item.is_last;
      } else {
        pending_size_ -= item.data.size();
      }
      return;
    }
  }
  buffer_.insert(insert_iter, item);
  if (buffer_[0].first_index == current_index_) {
    auto& to_write_item = buffer_[0];
    output_.writer().push(to_write_item.data);
    pending_size_ -= to_write_item.data.size();
    current_index_ = to_write_item.last_index;
    if (to_write_item.is_last) {
      output_.writer().close();
    }
    buffer_.erase(buffer_.begin());
  }
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return pending_size_;
}
