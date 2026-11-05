const canvas = document.getElementById("myCanvas");
const ctx = canvas.getContext('2d');

const width = window.innerWidth;
const height = window.innerHeight;

const halfWidth = width / 2;
const halfHeight = height / 2;

canvas.width = width;
canvas.height = height;

const matrix = Array.from({ length: height }, () => Array(width).fill(0));
const display_matrix = Array.from({ length: height }, () => Array(width).fill(0));

const imageData = ctx.createImageData(width, height);
const data = imageData.data;

function getRandomInt(max) {
    return Math.floor(Math.random() * max);
}

function compose_data(){
    let idx;
    let color;

    for (let y = 0; y < height; y++){
        for(let x = 0; x < width; x++){
            idx = (width * y + x) * 4;
            color = display_matrix[y][x] === 0 ? 0 : 255;

            data[idx]     = color;
            data[idx + 1] = color;
            data[idx + 2] = color;
            data[idx + 3] = 255;
        }
    }

    for (let i = 0;i<number_of_walkers;i++){
        idx = (width * p[i].y + p[i].x)*4;

        data[idx] = 255;
        data[idx + 1] = 0;
        data[idx + 2] = 0;
        data[idx + 3] = 255;
    }

}

const FPS = 60;

function start(){    
    compose_data();
    ctx.putImageData(imageData, 0, 0);


    for (let i = 0;i<number_of_walkers;i++){
        matrix[p[i].y][p[i].x] = 1 ;
        
        let action = getRandomInt(4);
        if (action === 0) 
            p[i].x = (p[i].x+1) % width;
        else if (action === 1) 
            p[i].y = (p[i].y+1) % height;
        else if (action === 2) 
            p[i].x = (p[i].x-1+width) % width;
        else if (action === 3) 
            p[i].y = (p[i].y-1+height) % height;
        
        matrix[p[i].y][p[i].x] = 1 ;
    }

    for (let i = 0; i < height; i++){
        for(let j = 0; j < width; j++){
            
            let decision = matrix[(i+1)%height][j] + 
                           matrix[(i + 1) % height][(j + 1) % width] + 
                           matrix[i][(j + 1) % width] + 
                           matrix[(i-1+height) % height][(j + 1) % width] + 
                           matrix[(i-1+height) % height][j] + 
                           matrix[(i-1+height) % height][(j - 1 + width) % width] + 
                           matrix[i][(j - 1 + width) % width] + 
                           matrix[(i + 1) % height][(j - 1 + width) % width];

            if(decision < 2){
                display_matrix[i][j] = 0;
            }
            else{
                if(matrix[i][j] === 1){
                    display_matrix[i][j] = (decision <= 3) ? 1 : 0;
                }
                else{
                    if(decision === 3){
                        display_matrix[i][j] = 1;
                    } 
                    else {
                        display_matrix[i][j] = 0;
                    }
                }
            }
        }
    }
    
    for (let i = 0; i < height; i++){
        for(let j = 0; j < width; j++){
            matrix[i][j] = display_matrix[i][j];
        }
    }
    
    setTimeout(start, 1000 / FPS);
}
    
let p=new Array();

let number_of_cells = Math.floor((height * width) * 0.1); 
let number_of_walkers = Math.floor((height * width) * 0.001); 
    
let R = Math.min(width, height) / 2;

for (let i = 0; i < number_of_cells; i++){
    let r = R * Math.sqrt(Math.random());
    let theta = Math.random() * 2 * Math.PI;

    let x = Math.floor(halfWidth + r * Math.cos(theta)); 
    let y = Math.floor(halfHeight + r * Math.sin(theta));

    matrix[(y+height)%height][(x+width)%width] = 1;
    display_matrix[(y+height)%height][(x+width)%width] = 1;
}

for (let i = 0; i < number_of_walkers; i++){
    let r = R * Math.sqrt(Math.random());
    let theta = Math.random() * 2 * Math.PI;

    let x = Math.floor(halfWidth + r * Math.cos(theta)); 
    let y = Math.floor(halfHeight + r * Math.sin(theta));

    p[i] = { y:(y+height)%height, x: (x+width)%width };
}

start();
