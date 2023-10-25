function drawMap(map){
    // Get the canvas element
    var canvas = document.getElementById("myCanvas");
    var context = canvas.getContext("2d");

    // Define your map data
    // var map = [
    //     [1, 2, 1, 1, 1, 1, 1, 1, 1],
    //     [1, 0, 1, 0, 0, 0, 1, 0, 1],
    //     [1, 0, 1, 0, 1, 0, 1, 0, 1],
    //     [1, 0, 0, 0, 1, 0, 1, 0, 1],
    //     [1, 1, 1, 1, 1, 0, 1, 0, 1],
    //     [1, 0, 0, 0, 1, 0, 1, 0, 1],
    //     [1, 0, 1, 0, 1, 0, 1, 0, 1],
    //     [1, 0, 1, 0, 0, 0, 0, 0, 1],
    //     [1, 0, 1, 1, 1, 1, 1, 1, 1],
    //     [1, 0, 0, 0, 0, 0, 0, 0, 1],
    //     [1, 1, 1, 1, 1, 1, 1, 2, 1]
    // ];

    // Define the size of each square
    var squareSize = 20;

    // Loop through the map and draw squares
    for (var i = 0; i < map.length; i++) {
        for (var j = 0; j < map[i].length; j++) {
        var x = j * squareSize;
        var y = i * squareSize;

        switch (map[i][j]) {
            case 0:
                context.fillStyle = "white"; // Path
                break;
            case 1:
                context.fillStyle = "orange"; // Wall
                break;
            case 2:
                context.fillStyle = "blue"; // Starting color
                break;
        }

        context.fillRect(x, y, squareSize, squareSize);
        }
    }
}

document.addEventListener("DOMContentLoaded", function () {
    var map1 = $(".map-data:contains('Map1')").text();
    var map2 = $(".map-data:contains('Map2')").text();
    var map3 = $(".map-data:contains('Map3')").text();
    var map4 = $(".map-data:contains('Map4')").text();

    // Remove the "MapX:" prefix
    map1 = map1.replace("Map1:", "").trim();

    // Check if there is any map
    if(map1){
        map2 = map2.replace("Map2:", "").trim();
        map3 = map3.replace("Map3:", "").trim();
        map4 = map4.replace("Map4:", "").trim();
    
        // Combine the map data
        var combinedMapData = map1 + " " + map2 + " " + map3 + " " + map4;
        
        drawMap(JSON.parse(combinedMapData));
    }
});