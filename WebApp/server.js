var fs = require('fs')
    , http = require('http'),
    socketio = require('socket.io'),
    url = require("url"),
    SerialPort = require("serialport").SerialPort

var express = require("express");
var path = require('path');
var mysql = require("mysql");
var app = express();
var httpServer;
var socketServer;
var serialPort;
var portName = 'COM24'; //change this to your Arduino port
var sendData = "";

var connection = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "",
    database: "smartsarajevo"
});

app.use(express.static(path.join(__dirname, 'public')));

connection.connect(function (error) {
    if (error) {
        console.log("Problem with MySQL" + error);
    }
    else {
        console.log("Connected with Database");
    }
});

app.listen(3000, function () {
    console.log("It's Started on PORT 3000");
});

app.get('/', function (req, res) {
    res.sendFile(path.join(__dirname, '/views/index.html'));

});
/*
 * Here we will call Database.
 * Fetch news from table.
 * Return it in JSON.
 */
app.get('/load', function (req, res) {
    connection.query("SELECT Distinct sl.id, sl.latitude, sl.longitude, sr.Temperature, sr.Humidity, sr.Pollution from sensorlocation sl Inner Join sensorreading sr on sl.ID = sr.LocationID order by TimeCreated DESC", function (err, rows) {
        if (err) {
            console.log("Problem with MySQL" + err);
        }
        else {
            res.end(JSON.stringify(rows));
        }
    });
});

app.get('/loadAllReadings',function(req,res){
    connection.query("SELECT * from sensorreading order by ID, TimeCreated",function(err,rows){
        if(err)
        {
            console.log("Problem with MySQL"+err);
        }
        else
        {
            res.end(JSON.stringify(rows));
        }
    });
});
// handle contains locations to browse to (vote and poll); pathnames.
function startServer(route, handle, debug) {
    // on request event
    function onRequest(request, response) {
        // parse the requested url into pathname. pathname will be compared
        // in route.js to handle (var content), if it matches the a page will
        // come up. Otherwise a 404 will be given.
        var pathname = url.parse(request.url).pathname;
        console.log("Request for " + pathname + " received");
        var content = route(handle, pathname, response, request, debug);
    }

    httpServer = http.createServer(onRequest).listen(1337, function () {
        console.log("Listening at: http://localhost:1337");
        console.log("Server is up");
    });
    serialListener(debug);
    initSocketIO(httpServer, debug);
}

function initSocketIO(httpServer, debug) {
    socketServer = socketio.listen(httpServer);
    if (debug == false) {
        socketServer.set('log level', 1); // socket IO debug off
    }
    socketServer.on('connection', function (socket) {
        console.log("user connected");
        socket.emit('onconnection', {pollOneValue: sendData});
        socketServer.on('update', function (data) {
            socket.emit('updateData', {pollOneValue: data});
        });
        socket.on('buttonval', function (data) {
            serialPort.write(data + 'E');
        });
    });
}

function SocketIO_serialemit(sendData) {
    // console.log("serial emit: ",sendData);
    //	connection.query("insert into sensorlocation (Latitude, Longitude) values (23, 32)");
    socketServer.emit('updateData', {pollOneValue: sendData});
    //socketServer.emit('update', sendData);
}


// Listen to serial port
function serialListener(debug) {
    var receivedData = "";
    serialPort = new SerialPort(portName, {
        baudrate: 9600,
        // defaults for Arduino serial communication
        dataBits: 8,
        parity: 'none',
        stopBits: 1,
        flowControl: false
    });

    serialPort.on("open", function () {
        console.log('open serial communication');
        // Listens to incoming data
        serialPort.on('data', function (data) {
            receivedData += data.toString();
			console.log(receivedData);
            if (receivedData.indexOf('E') >= 0 && receivedData.indexOf('B') >= 0) {
                var sendData = receivedData.substring(receivedData.indexOf('B') + 1, receivedData.indexOf('E'));
                if(receivedData.indexOf('E') != receivedData.length - 1)
                    receivedData = receivedData.substring(receivedData.indexOf('E') + 1, receivedData.length);
                else
                    receivedData = "";

				var splitedData = sendData.split('T');
				var temperature = parseInt(splitedData[0]);
				var humidity = parseInt(splitedData[1].split('H')[0]);
				var pollution = parseFloat(sendData.split('H').reverse()[0]);

                console.log("T:" + temperature + ";   H" + humidity + ";  P" + pollution);
                connection.query("Insert into sensorreading (LocationID, Temperature, Pollution, Humidity) values (1," + temperature + ", " + pollution + ", " + humidity + ")");
            }
            SocketIO_serialemit(sendData);
        });
    });
}

exports.start = startServer;