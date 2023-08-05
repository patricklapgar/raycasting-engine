/* Prototype raycasting engine implement in JavaScript */

const TILE_SIZE = 64;
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;

const WINDOW_WIDTH = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;

const FOV_ANGLE = 60 * (Math.PI / 180);
const WALL_STRIP_WIDTH = 15;
const NUM_RAYS = WINDOW_WIDTH / WALL_STRIP_WIDTH;

const MINIMAP_SCALE_FACTOR = 0.2;

class Map {
    constructor() {
        this.grid = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        ];
    }

    isWall(x, y) {
        if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
            return false;

        var xIndex = Math.floor(x / TILE_SIZE);
        var yIndex = Math.floor(y / TILE_SIZE);
        return this.grid[yIndex][xIndex] != 0;
    }

    getWallContent(x,  y) {
        if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
            return 0;
        
        var xIndex = Math.floor(x / TILE_SIZE);
        var yIndex = Math.floor(y / TILE_SIZE);
        return this.grid[yIndex][xIndex];
    }

    render() {
        for (var i = 0; i < MAP_NUM_ROWS; i++) {
            for (var j = 0; j < MAP_NUM_COLS; j++) {
                var tileX = j * TILE_SIZE; 
                var tileY = i * TILE_SIZE;
                var tileColor = this.grid[i][j] == 1 ? "#222" : "#fff";
                stroke("#222");
                fill(tileColor);
                rect(
                    MINIMAP_SCALE_FACTOR * tileX, 
                    MINIMAP_SCALE_FACTOR * tileY, 
                    MINIMAP_SCALE_FACTOR * TILE_SIZE, 
                    MINIMAP_SCALE_FACTOR * TILE_SIZE
                );
            }
        }
    }
}

class Player {
    constructor() {
        this.x = WINDOW_WIDTH / 2;
        this.y = WINDOW_HEIGHT / 2;
        this.radius = 3; // in pixels
        this.turnDirection = 0; // -1 if left, +1 if right
        this.walkDirection = 0; // -1 if backwards, +1 if forwards
        this.rotationAngle = Math.PI / 2;
        this.moveSpeed = 3.0; // pixels/frame
        this.rotationSpeed = 5 * (Math.PI / 180);
    }

    update() {
        this.rotationAngle += this.turnDirection * this.rotationSpeed;

        var displacement = this.walkDirection * this.moveSpeed;
        var newXPos = this.x + Math.cos(this.rotationAngle) * displacement;
        var newYPos = this.y + Math.sin(this.rotationAngle) * displacement;

        if(!grid.isWall(newXPos, newYPos)) {
            this.x = newXPos;
            this.y = newYPos;
        }
    }

    render() {
        noStroke();
        fill("red");
        circle(
            MINIMAP_SCALE_FACTOR * this.x, 
            MINIMAP_SCALE_FACTOR * this.y, 
            MINIMAP_SCALE_FACTOR * this.radius
        );
        stroke("red");
    }
}

class Ray {
    constructor(angle) {
        this.rayAngle = normalizeAngle(angle);
        this.wallHitX = 0;
        this.wallHitY = 0;
        this.distance = 0;
        this.wasHitVertical = false;
        this.hitWallColor = 0;

        this.isRayFaceDown = this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.isRayFaceUp = !this.isRayFaceDown;

        this.isRayFaceRight = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.isRayFaceLeft = !this.isRayFaceRight;
    }

    cast() {
        var xintercept, yintercept;
        var xstep, ystep;
        // Horizontal ray-grid intersection
        
        var foundHorizontalWall = false;
        var horizontalWallHitX = 0;
        var horizontalWallHitY = 0;
        var horizontalWallColor = 0;

        // Find the y-coordinate of the closest horizontal grid intersection
        yintercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        yintercept += this.isRayFaceDown ? TILE_SIZE : 0;

        // Find the x-coordinate of the closest horizontal grid intersection
        xintercept = player.x + ((yintercept - player.y) / Math.tan(this.rayAngle));
        
        // Calculate increment for xstep and ystep
        ystep = TILE_SIZE;
        ystep *= this.isRayFaceUp ? -1 : 1;

        xstep = TILE_SIZE / Math.tan(this.rayAngle);
        xstep *= (this.isRayFaceLeft && xstep > 0) ? -1 : 1;
        xstep *= (this.isRayFaceRight && xstep < 0) ? -1 : 1;

        var nextHorizontalX = xintercept;
        var nextHorizontalY = yintercept;
        
        // Increment xstep and ystep until a wall is hit
        while (nextHorizontalX >= 0 && nextHorizontalX <= WINDOW_WIDTH && nextHorizontalY >= 0 && nextHorizontalY <= WINDOW_HEIGHT) {
            var wallGridContent = grid.getWallContent(
                nextHorizontalX,
                nextHorizontalY + (this.isRayFaceUp ? -1 : 0)
            );

            if (wallGridContent != 0) {
                foundHorizontalWall = true;
                horizontalWallHitX = nextHorizontalX;
                horizontalWallHitY = nextHorizontalY;
                horizontalWallColor = wallGridContent;
                break;
            } else {
                nextHorizontalX += xstep;
                nextHorizontalY += ystep;
            }
        }

        var foundVerticalWall = false;
        var verticalWallHitX = 0;
        var verticalWallHitY = 0;
        var verticalWallColor = 0;

        // Find the x-coordinate of the closest vertical grid intersection
        xintercept = Math.floor(player.x / TILE_SIZE) * TILE_SIZE;
        xintercept += this.isRayFaceRight ? TILE_SIZE : 0;

        // Find the y-coordinate of the closest vertical grid intersection
        yintercept = player.y + (xintercept - player.x) * Math.tan(this.rayAngle);
        
        // Calculate increment for xstep and ystep
        xstep = TILE_SIZE;
        xstep *= this.isRayFaceLeft ? -1 : 1;

        ystep = TILE_SIZE * Math.tan(this.rayAngle);
        ystep *= (this.isRayFaceUp && ystep > 0) ? -1 : 1;
        ystep *= (this.isRayFaceDown && ystep < 0) ? -1 : 1;

        var nextVerticalX = xintercept;
        var nextVerticalY = yintercept;
        
        // Increment xstep and ystep until a wall is hit
        while (nextVerticalX >= 0 && nextVerticalX <= WINDOW_WIDTH && nextVerticalY >= 0 && nextVerticalY <= WINDOW_HEIGHT) {
            var wallGridContent = grid.getWallContent(
                nextVerticalX + (this.isRayFaceLeft ? -1 : 0),
                nextVerticalY
            );

            if (wallGridContent != 0) {
                foundVerticalWall = true;
                verticalWallHitX = nextVerticalX;
                verticalWallHitY = nextVerticalY;
                verticalWallColor = wallGridContent;
                break;
            } else {
                nextVerticalX += xstep;
                nextVerticalY += ystep;
            }
        }

        // Calculate both horizontal and vertical distances and choose the smallest value
        var horizontalHitDistance = (foundHorizontalWall) 
            ? distanceBetweenPoints(player.x, player.y, horizontalWallHitX, horizontalWallHitY)
            : Number.MAX_VALUE;
        
        var verticalHitDistance = (foundVerticalWall)
            ? distanceBetweenPoints(player.x, player.y, verticalWallHitX, verticalWallHitY)
            : Number.MAX_VALUE;
        
        if (verticalHitDistance < horizontalHitDistance) {
            this.wallHitX = verticalWallHitX;
            this.wallHitY = verticalWallHitY;
            this.distance = verticalHitDistance;
            this.hitWallColor = verticalWallColor;
            this.wasHitVertical = true;
        } else {
            this.wallHitX = horizontalWallHitX;
            this.wallHitY = horizontalWallHitY;
            this.distance = horizontalHitDistance;
            this.hitWallColor = horizontalWallColor;
            this.wasHitVertical = false;
        }
    }

    render() {
        stroke("rgba(255, 0, 0, 0.3)");
        line(
            MINIMAP_SCALE_FACTOR * player.x, 
            MINIMAP_SCALE_FACTOR * player.y, 
            MINIMAP_SCALE_FACTOR * this.wallHitX,
            MINIMAP_SCALE_FACTOR * this.wallHitY
        );
    }
}


var grid = new Map();
var player = new Player();
var rays = [];

function keyPressed() {
    switch(keyCode) {
        case UP_ARROW:
            player.walkDirection = 1;
            console.log("PRESSED UP");
            break;
        case DOWN_ARROW:
            player.walkDirection = -1;
            console.log("PRESSED DOWN");
            break;
        case RIGHT_ARROW:
            player.turnDirection = 1;
            console.log("PRESSED RIGHT");
            break;
        case LEFT_ARROW:
            player.turnDirection = -1;
            console.log("PRESSED LEFT");
            break;
        default:
            console.log("No arrow key pressed...");
            break;
    }
}

function keyReleased() {
    switch(keyCode) {
        case UP_ARROW:
            player.walkDirection = 0;
            console.log("RELEASED UP");
            break;
        case DOWN_ARROW:
            player.walkDirection = 0;
            console.log("RELEASED DOWN");
            break;
        case RIGHT_ARROW:
            player.turnDirection = 0;
            console.log("RELEASED RIGHT");
            break;
        case LEFT_ARROW:
            player.turnDirection = 0;
            console.log("RELEASED LEFT");
            break;
        default:
            console.log("No arrow key pressed...");
            break;
    }
}

function castAllRays() {
    var rayAngle = player.rotationAngle - (FOV_ANGLE / 2);
    rays = [];

    for(var i = 0; i < NUM_RAYS; i++) {
        var ray = new Ray(rayAngle);
        ray.cast();
        rays.push(ray);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

function renderCeiling() {
    noStroke();
    fill("#414141");
    rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT / 2);
}

function renderFloor() {
    noStroke();
    fill("#818181");
    rect(0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT);
}

function renderProjectedWalls() {
    renderCeiling();
    renderFloor();

    // Render all rays
    for (var i = 0; i < NUM_RAYS; i++) {
        var currRay = rays[i];

        // Do this to prevent fishbowl distortion
        var correctDistance = currRay.distance * Math.cos(currRay.rayAngle - player.rotationAngle);

        var distanceProjectionPlane = (WINDOW_WIDTH / 2) / Math.tan(FOV_ANGLE / 2);
        
        // How tall the projected wall will be
        var wallStripHeight = (TILE_SIZE / correctDistance) * distanceProjectionPlane;
        
        var color = currRay.wasHitVertical ? 255 : 200;

        var colorR = currRay.hitWallColor == 1 ? color : 0;
        var colorG = currRay.hitWallColor == 2 ? color : 0;
        var colorB = currRay.hitWallColor == 3 ? color : 0;
        var alpha = 1.0;

        fill("rgba(" + colorR + ", " + colorG + ", " + colorB + ", " + alpha + ")");
        noStroke();

        rect(
            i * WALL_STRIP_WIDTH,
            (WINDOW_HEIGHT / 2) - (wallStripHeight / 2),
            WALL_STRIP_WIDTH,
            wallStripHeight
        );
    }
}

function normalizeAngle(angle) {
    angle = angle % (2 * Math.PI);
    if(angle < 0) {
        angle += (2 * Math.PI);
    }

    return angle;
}

function distanceBetweenPoints(x1, y1, x2, y2) {
    return Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

function setup() {
    createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
}

function update() {
    player.update();
    castAllRays();
}

function draw() {
    clear("#111");
    update();
    
    renderProjectedWalls();

    grid.render();
    for(ray of rays) {
        ray.render();
    }
    player.render();
}
