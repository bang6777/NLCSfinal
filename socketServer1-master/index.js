const http = require('http');
var express = require("express");
var app = express();
const hostname = '192.168.0.100';
const port = 3000;
app.use(express.static("public"));
app.set("view engine", "ejs");
app.set("views", "./views");

var server = require("http").Server(app);
var io = require("socket.io")(server);


// server = http.createServer((req, res) => {
//   res.statusCode = 200;
//   res.setHeader('Content-Type', 'text/plain');
//   res.end('Hello World\n');
// });
//
// io.sockets.on('connection', function(socket){
//   console.log('a user connected' + socket.id);
// });

io.on('connection', function(socket){
  console.log('a user connected ' + socket.id);
  socket.on('disconnect',function(){
    console.log('a user disconnected ' + socket.id);
  });
  socket.on('Client-send-connect',function(data){
    console.log('Node MCU send: ' + data);
  });

  socket.on("valueRain",function(data){
    console.log(socket.id+"vua gui" + data );
    io.sockets.emit("Server-send-valueRain",data);
  });
  socket.on("value",function(data){
    console.log(socket.id+"vua gui" + data );
    io.sockets.emit("Server-send-value",data);
  });


  socket.on("Client-send-beginSend",function(data){
    console.log(socket.id+"vua gui "+data);
    socket.broadcast.emit("Server-send-turnOn");
  });

  socket.on("Client-send-turnOn",function(data){
    console.log("Server vua gui ON:"+data);
    io.sockets.emit("Server-send-turnOn",data);
    io.sockets.emit("Client-send-beginSend",data);


  });
  socket.on("Client-send-turnOff",function(data){
    console.log(socket.id+"vua gui "+data);
    io.sockets.emit("Server-send-turnOff",data);
  });
  socket.on("Client-send-stop",function(data){
    console.log(socket.id+"vua gui "+data);
    io.sockets.emit("Server-send-stop",data);
  });
  socket.on("sent",function(data){
    console.log(socket.id+"vua gui " +data);
    io.sockets.emit("Arduino-send",data);
  });
});


// server.listen(3000);
server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});
app.get("/", function(req,res){
  res.render("trangchu",{hostname: hostname, port:port} );

});
