#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) :
  capacity_( capacity ), buffer_(), amount_(0), total_pushed_(0),
  total_poped_(0), first_string_left_size(0), close_( false ), error_( false )  {}

bool Writer::is_closed() const
{
  // Your code here.
  return close_;
}

void Writer::push( string data )
{
  // Your code here.
  uint64_t free_capacity = available_capacity();
  uint64_t to_push_size = min(free_capacity, data.size());
  if(to_push_size == 0)  return;
  data.resize(to_push_size);
  buffer_.emplace(std::move(data));
  if(buffer_.size() == 1)
    first_string_left_size = to_push_size;
  total_pushed_ += to_push_size;
  amount_ += to_push_size;
  return;
}

void Writer::close()
{
  // Your code here.
    close_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - amount_;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return total_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return amount_ == 0 && close_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return total_poped_;
}

string_view Reader::peek() const
{
  // Your code here.
  if(amount_ == 0 || buffer_.empty()){
    return string_view{};
  }
  const string& front = buffer_.front();
//  return string_view(front.data()+front.size()-first_string_left_size,1);
//  return string_view(&front[front.size()-first_string_left_size]);
  return string_view(front).substr(front.size()-first_string_left_size);
}


void Reader::pop( uint64_t len )
{
  // Your code here.
  total_poped_ += len;
  amount_ -= len;
  while(len){
    if(len >= first_string_left_size){
      len -= first_string_left_size;
      buffer_.pop();
      first_string_left_size = buffer_.front().size();
    } else{
      first_string_left_size -= len;
      len = 0;
    }
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return amount_;
}
