const canvas = document.getElementById("myCanvas");
const ctx = canvas.getContext('2d');

// 1. Cache dimensions once
const width = window.innerWidth;
const height = window.innerHeight;

const halfWidth = width / 2;
const halfHeight = height / 2;

canvas.width = width;
canvas.height = height;

const imageData = ctx.createImageData(width, height);
const data = imageData.data;

function getRandomInt(max) {
    return Math.floor(Math.random() * max);
}

function draw_point({x,y,trail},i){

    x = Math.floor(x + width / 2);
    y = Math.floor(y + height / 2);

    if (x >= width || x < 0 || y >= height || y < 0) return;

    let idx = (width * y + x) * 4;
    
    let num = ((1 << 24) + (data[idx] << 16) + (data[idx+1] << 8) + data[idx+2] + (i<<12));
    
    data[idx  ] = (num >> 16) & 0xff;
    data[idx+1] = (num >> 8 ) & 0xff;
    data[idx+2] = num && 0xff;
    data[idx+3] = 255;

    trail.push({x:x,y:y});

    if(trail.length === trail_length){
        let coords = trail.shift();

        x = coords.x;
        y = coords.y;
        
        idx = (width * y + x) * 4;
        
        data[idx] = 29;     // R
        data[idx + 1] = 32; // G
        data[idx + 2] = 33; // B
        data[idx + 3] = 255; // Alpha (fully opaque)
    

    }

}

const FPS = 60;

function start(){    
    for (let i=0;i<number_of_walkers;i++){

        let action = getRandomInt(4);

        if (action === 0) 
            p[i].x++;
        else if (action === 1) 
            p[i].y++;
        else if (action === 2) 
            p[i].x--;
        else if (action === 3) 
            p[i].y--;

        if (p[i].x < -halfWidth) {
            p[i].x = halfWidth - 1;
        } else if (p[i].x >= halfWidth) {
            p[i].x = -halfWidth;
        }

        if (p[i].y < -halfHeight) {
            p[i].y = halfHeight - 1;
        } else if (p[i].y >= halfHeight) {
            p[i].y = -halfHeight;
        }

        draw_point(p[i],i);
    
    }

    ctx.putImageData(imageData, 0, 0);
    
    setTimeout(start, 1000/FPS);
}

let number_of_walkers = Math.floor(height*width / 500);
console.log(number_of_walkers);

let trail_length = 250;

let p=new Array();

for (let i = 0; i < number_of_walkers; i++){
    let R = Math.min(width, height) / 2;
    let r = R * Math.sqrt(Math.random());
    let theta = Math.random() * 2 * Math.PI;

    let x = Math.floor(r * Math.cos(theta)); 
    let y = Math.floor(r * Math.sin(theta));
    
    p[i] = {x:x, y:y, trail : new Array()};

}

for (let i = 0; i < data.length; i += 4) {
    data[i] = 29;     // R
    data[i + 1] = 32; // G
    data[i + 2] = 33; // B
    data[i + 3] = 255; // Alpha (fully opaque)
}
ctx.putImageData(imageData, 0, 0);

start();
