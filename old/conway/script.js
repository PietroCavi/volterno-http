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

function compose_data(){
    for (let y = 0; y < height; y++){
        for(let x = 0; x < width; x++){
            let idx = (width * y + x) * 4;
            let color = display_matrix[y][x] === 0 ? 0 : 255;

            data[idx]     = color;
            data[idx + 1] = color;
            data[idx + 2] = color;
            data[idx + 3] = 255;
        }
    }
}

const FPS = 60;

function start(){    
    compose_data();
    ctx.putImageData(imageData, 0, 0);

    for (let i = 0; i < height; i++){
        for(let j = 0; j < width; j++){
            
            let decision = matrix[(i+1+height)%height][j] + 
                           
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
                        display_matrix[i][j] = 0;                     }
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

let number_of_cells = Math.floor((height * width) * 0.07); 

for (let i = 0; i < number_of_cells; i++){
    let R = Math.min(width, height) / 2;
    let r = R * Math.sqrt(Math.random());
    let theta = Math.random() * 2 * Math.PI;

    let x = Math.floor(halfWidth + r * Math.cos(theta)); 
    let y = Math.floor(halfHeight + r * Math.sin(theta));

    if (x >= 0 && x < width && y >= 0 && y < height) {
        matrix[y][x] = 1;
        display_matrix[y][x] = 1;
    }
}

start();
