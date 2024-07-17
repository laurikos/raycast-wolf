const TILE_SIZE = 64;
const MAP_ROWS = 11;
const MAP_COLS = 15;

const WINDOW_WIDTH = TILE_SIZE * MAP_COLS;
const WINDOW_HEIGHT = TILE_SIZE * MAP_ROWS;

// FOV 60 degrees in radians
const FOV_ANGLE = 60 * (Math.PI / 180);

const WALL_STRIP_WIDTH = 1;
const NUM_RAYS = WINDOW_WIDTH / WALL_STRIP_WIDTH;

class Grid {
  constructor() {
    this.layout = [
      [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
      [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
      [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
      [1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1],
      [1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
      [1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
      [1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1],
      [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
      [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
    ];
  }

  get(x, y) {
    if (x < 0 || y < 0 || x >= MAP_COLS || y >= MAP_ROWS) {
      console.warn("grid::get(x, y) => out of bounds");
      return 1;
    }

    return this.layout[y][x];
  }

  render() {
    stroke("#222");

    for (let y = 0; y < MAP_ROWS; y++) {
      for (let x = 0; x < MAP_COLS; x++) {

        let color = this.get(x, y) === 1 ? "#222" : "#fff";

        fill(color);

        rect(
          x * TILE_SIZE,
          y * TILE_SIZE,
          TILE_SIZE,
          TILE_SIZE
        );


      }
    }
  }
}

const TURN_DIRECTIONS = {
  RIGHT: 1,
  LEFT: -1,
  NONE: 0
};

const MOVE_DIRECTION = {
  FORWARD: 1,
  BACKWARD: -1,
  NONE: 0
};

const PlayerDirectionLineLen = 40;

class Player {
  constructor() {
    this.x = WINDOW_WIDTH / 2;
    this.y = WINDOW_HEIGHT / 2;
    this.radius = 6;
    this.moveSpeed = 2.0;
    this.rotationSpeed = 2 * (Math.PI / 180); // 3 degrees
    this.turnDirection = TURN_DIRECTIONS.NONE;
    this.moveDirection = MOVE_DIRECTION.NONE;
    this.rotationAngle = Math.PI / 2; // 90 degrees
  }

  getNewX(angle, distance) {
    return this.x + Math.cos(angle) * distance;
  }

  getNewY(angle, distance) {
    return this.y + Math.sin(angle) * distance;
  }

  render() {
    noStroke();
    fill("#e35473");
    circle(this.x, this.y, this.radius);
    stroke("#e35473");
    if (false) {
      line(
        // line start x position
        this.x,

        // line start y position
        this.y,

        // line end x position 20 pixels away from start
        this.getNewX(this.rotationAngle, PlayerDirectionLineLen),

        // line end y position 20 pixels away from start
        this.getNewY(this.rotationAngle, PlayerDirectionLineLen)
      );
    }
  }

  update() {
    this.rotationAngle += this.turnDirection * this.rotationSpeed;

    const moveStep = this.moveDirection * this.moveSpeed;

    const newPlayerX = this.getNewX(this.rotationAngle, moveStep);
    const newPlayerY = this.getNewY(this.rotationAngle, moveStep);

    if (grid.get(
      Math.floor(newPlayerX / TILE_SIZE),
      Math.floor(newPlayerY / TILE_SIZE)
    ) === 0) {
      this.x = newPlayerX;
      this.y = newPlayerY;
    }
  }
}


function distanceBetweenPoints(x1, y1, x2, y2) {
  return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
}

class Ray {
  constructor(rayAngle) {
    this.rayAngle = normalizeAngle(rayAngle);
    this.wallHitX = 0;
    this.wallHitY = 0;
    this.distance = 0;
    this.wasHitVertical = false;

    // notice that we use the unit circle counter-clockwise here
    // So if the angle is between 0 and 180 degrees, the ray is facing down
    // so between 0 and pi
    this.isRayFacingDown = this.rayAngle > 0 && this.rayAngle < Math.PI;
    this.isRayFacingUp = !this.isRayFacingDown;

    // And if the angle is between 90 and 270 degrees, the ray is facing right
    // so between pi/2 and 3pi/2
    this.isRayFacingRight = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
    this.isRayFacingLeft = !this.isRayFacingRight;

  }

  getRayAngle() {
    return this.rayAngle;
  }

  getFacing() {
    result = ""
    if (this.isRayFacingDown) {
      result += "DOWN";
    }
    if (this.isRayFacingUp) {
      result += "UP";
    }
    if (this.isRayFacingRight) {
      result += "RIGHT";
    }
    if (this.isRayFacingLeft) {
      result += "LEFT";
    }

    return result;
  }

  cast(columndID) {
    const resultHorizontal = this.castHorizontalIntersection();
    const resultVertical = this.castVerticalIntersection();

    let horizontalHitDistance, verticalHitDistance;

    if (resultHorizontal.found) {

      horizontalHitDistance = distanceBetweenPoints(
        player.x,
        player.y,
        resultHorizontal.x,
        resultHorizontal.y
      );

    } else {
      horizontalHitDistance = Number.MAX_VALUE;
    }

    if (resultVertical.found) {

      verticalHitDistance = distanceBetweenPoints(
        player.x,
        player.y,
        resultVertical.x,
        resultVertical.y
      );

    } else {
      verticalHitDistance = Number.MAX_VALUE;
    }

    if (horizontalHitDistance < verticalHitDistance) {
      this.wallHitX = resultHorizontal.x;
      this.wallHitY = resultHorizontal.y;
      this.distance = horizontalHitDistance;
    } else {
      this.wallHitX = resultVertical.x;
      this.wallHitY = resultVertical.y;
      this.distance = verticalHitDistance;
      this.wasHitVertical = true;
    }

  }

  castHorizontalIntersection() {
    let xintercept, yintercept;
    let xstep, ystep;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
    if (this.isRayFacingDown) {
      // Also notice that we are using the grid coordinates from the array
      // so not like, say OpenGL would use the coordinate system for screen.
      // hence when the ray is facing down, the y-intercept is the y-coordinate of the bottom grid
      // and we need to add the tile size.
      yintercept += TILE_SIZE;
    }

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = player.x + (yintercept - player.y) / Math.tan(this.rayAngle);

    ystep = TILE_SIZE;
    if (this.isRayFacingUp) {
      ystep *= -1;
    }

    xstep = TILE_SIZE / Math.tan(this.rayAngle);

    if (this.isRayFacingLeft && xstep > 0) {
      xstep *= -1;
    }

    if (this.isRayFacingRight && xstep < 0) {
      xstep *= -1;
    }

    let nextHorizontalGridX = xintercept;
    let nextHorizontalGridY = yintercept;

    let foundHorizontalWallHit = false;
    let horizontalWallHitX = 0;
    let horizontalWallHitY = 0;

    while (
      nextHorizontalGridX >= 0 &&
      nextHorizontalGridX < WINDOW_WIDTH &&
      nextHorizontalGridY >= 0 &&
      nextHorizontalGridY < WINDOW_HEIGHT
    ) {

      // If we are casting ray upwards we can decrement the Y value by one
      // (again this is because of the coordinate system we are using)
      // to see if we are inside of a wall rather than on the edge pixel. 
      let gridY = nextHorizontalGridY;
      if (this.isRayFacingUp) {
        gridY -= 1;
      }

      if (grid.get(
        Math.floor(nextHorizontalGridX / TILE_SIZE),
        Math.floor(gridY / TILE_SIZE)
      ) === 1) {
        foundHorizontalWallHit = true;
        horizontalWallHitX = nextHorizontalGridX;
        horizontalWallHitY = nextHorizontalGridY;

        break;
      } else {
        nextHorizontalGridX += xstep;
        nextHorizontalGridY += ystep;
      }

    }

    return {
      "found": foundHorizontalWallHit,
      "x": horizontalWallHitX,
      "y": horizontalWallHitY
    };
  }

  castVerticalIntersection() {
    let xintercept, yintercept;
    let xstep, ystep;

    // Find the x-coordinate of the closest vertical grid intersection
    xintercept = Math.floor(player.x / TILE_SIZE) * TILE_SIZE;
    if (this.isRayFacingRight) {
      // Also notice that we are using the grid coordinates from the array
      // so not like, say OpenGL would use the coordinate system for screen.
      // hence when the ray is facing down, the y-intercept is the y-coordinate of the bottom grid
      // and we need to add the tile size.
      xintercept += TILE_SIZE;
    }

    // Find the y-coordinate of the closest vertical grid intersection
    yintercept = player.y + (xintercept - player.x) * Math.tan(this.rayAngle);

    xstep = TILE_SIZE;
    if (this.isRayFacingLeft) {
      xstep *= -1;
    }

    ystep = TILE_SIZE * Math.tan(this.rayAngle);

    if (this.isRayFacingUp && ystep > 0) {
      ystep *= -1;
    }

    if (this.isRayFacingDown && ystep < 0) {
      ystep *= -1;
    }

    let nextVerticalGridX = xintercept;
    let nextVerticalGridY = yintercept;

    let foundVerticalWallHit = false;
    let verticalWallHitX = 0;
    let verticalWallHitY = 0;

    while (
      nextVerticalGridX >= 0 &&
      nextVerticalGridX < WINDOW_WIDTH &&
      nextVerticalGridY >= 0 &&
      nextVerticalGridY < WINDOW_HEIGHT) {

      // if we are casting the ray to the left we can decrement the x value by one
      // (again this is because of the coordinate system we are using)
      // to see if we are inside of a wall rather than on the edge pixel.
      let gridX = nextVerticalGridX;
      if (this.isRayFacingLeft) {
        gridX -= 1;
      }
      if (grid.get(
        Math.floor(gridX / TILE_SIZE),
        Math.floor(nextVerticalGridY / TILE_SIZE)
      ) === 1) {
        foundVerticalWallHit = true;
        verticalWallHitX = nextVerticalGridX;
        verticalWallHitY = nextVerticalGridY;

        break;
      } else {
        nextVerticalGridX += xstep;
        nextVerticalGridY += ystep;
      }

    }

    return {
      "found": foundVerticalWallHit,
      "x": verticalWallHitX,
      "y": verticalWallHitY
    };
  }

  render() {
    // RGB 181, 50, 219, 1 
    stroke(color(181, 50, 219, 80));
    // stroke(color(84, 94, 72, 80));
    line(
      player.x,
      player.y,
      this.wallHitX,
      this.wallHitY,
    );
  }

  getWallHit() {
    return `x: ${this.wallHitX}, y: ${this.wallHitY}`
  }
}

function normalizeAngle(angle) {
  angle = angle % (2 * Math.PI);
  if (angle < 0.0) {
    angle = (2 * Math.PI) + angle;
  }

  return angle;
}

function keyPressed() {
  if (keyCode === LEFT_ARROW) {
    player.turnDirection = TURN_DIRECTIONS.LEFT;
  } else if (keyCode === RIGHT_ARROW) {
    player.turnDirection = TURN_DIRECTIONS.RIGHT;
  } else if (keyCode === UP_ARROW) {
    player.moveDirection = MOVE_DIRECTION.FORWARD;
  } else if (keyCode === DOWN_ARROW) {
    player.moveDirection = MOVE_DIRECTION.BACKWARD;
  }
}

function keyReleased() {
  if (keyCode === LEFT_ARROW || keyCode === RIGHT_ARROW) {
    player.turnDirection = TURN_DIRECTIONS.NONE;
  } else if (keyCode === UP_ARROW || keyCode === DOWN_ARROW) {
    player.moveDirection = MOVE_DIRECTION.NONE;
  }
}

function castRays() {
  let columnID = 0;

  let rayAngle = player.rotationAngle - (FOV_ANGLE / 2);

  rays = [];

  let debug = false;
  if (debug) {

    let rayAngle = player.rotationAngle;
    let ray = new Ray(rayAngle);
    ray.cast(columnID);
    rays.push(ray);
    columnID++;

    // console.log(`Ray angle: ${rayAngle}, Wall hit: ${ray.getWallHit()}, Facing: ${ray.getFacing()}, "Distance: ${ray.distance}, wasHitVertical: ${ray.wasHitVertical}`);
  } else {

    for (let i = 0; i < NUM_RAYS; i++) {
      const ray = new Ray(rayAngle);
      ray.cast(columnID);
      rays.push(ray);

      rayAngle += FOV_ANGLE / NUM_RAYS;

      columnID++;
    }
  }
}

// --------------------------------------------------------------------------------------------

const grid = new Grid();
const player = new Player();

let rays = [];

function setup() {
  createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
}

function update() {
  player.update();
  castRays();
}

function draw() {
  update();

  grid.render();

  rays.forEach(ray => {
    ray.render();
  });

  player.render();
}
