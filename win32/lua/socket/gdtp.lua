local socket = require("socket")
local json = require("JSON")

local M = {}

-- Function to create a new client connection
function M.create_client(host, port)
    local client = assert(socket.connect(host, port))
    client:settimeout(0)
    return client
end

-- Function to create a new server
function M.create_server(host, port)
    local server = assert(socket.bind(host, port))
    server:settimeout(0)
    return server
end

-- Function to send data to a client
function M.send_data(client, data)
    client:send(data .. "\r\n")
end

-- Function to receive data from a client with a timeout
function M.receive_data(client, timeout)
    client:settimeout(timeout)
    local data, err = client:receive()
    client:settimeout(0)
    return data, err
end

-- Function to encode data to JSON
function M.encode_json(data)
    return json:encode(data)
end

-- Function to decode JSON data
function M.decode_json(data)
    return json:decode(data)
end

return M