require 'socket'

IP = "192.168.0.163"

class Command
  FORWARD = 1
  REVERSE = 2
  RIGHT   = 3
  LEFT    = 4
  STOP    = 5

  HEADER = 0x00;

  attr_reader :code, :duration, :speed

  def initialize(code, duration = 1, speed = 255)
    @code, @duration, @speed = code, duration, speed
  end

  def checksum
    code ^ duration ^ speed
  end

  def to_b
    [HEADER, code, duration, speed, checksum].pack("CCCCC")
  end
end

socket = TCPSocket.new(IP, 333)

sleep 1
socket.write(Command.new(Command::FORWARD, 20, 255).to_b)
sleep 1
socket.write(Command.new(Command::REVERSE, 10, 255).to_b)
sleep 1
socket.write(Command.new(Command::LEFT, 5, 255).to_b)
sleep 1
socket.write(Command.new(Command::RIGHT, 5, 255).to_b)

socket.close
