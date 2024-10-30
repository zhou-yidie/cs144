#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return flag & ( 1 << CLOSED );
}

void Writer::push( string data )
{
  // Your code here.
  auto len = min( data.size() ,available_capacity() );
  if (len == 0) {
    return;
  } else if (len < data.size()) {
    data.resize(len);
  }
  buffer_data.push(move(data));
  if (buffer_data.size() == 1) {
    buffer_view = buffer_data.front();
  }
  bytes_pushed_ += len;
  return;
}

void Writer::close()
{
  // Your code here.
  flag |= ( 1 << CLOSED );
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - reader().bytes_buffered();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return writer().is_closed() && ( bytes_buffered() == 0 );
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return bytes_popped_;
}

string_view Reader::peek() const
{
  // Your code here.
  return buffer_view;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if (len > bytes_buffered()) {
    return;
  }
  bytes_popped_ += len;
  while ( len > 0 ) {
    if (len >= buffer_view.size() ) {
      len -= buffer_view.size();
      buffer_data.pop();
      buffer_view = buffer_data.front();
    } else {
      buffer_view.remove_prefix( len );
      len = 0;
    }
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return writer().bytes_pushed() - bytes_popped();
}
