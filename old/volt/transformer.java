import java.io.*;
import javax.imageio.*;
import java.awt.image.BufferedImage;
import java.awt.*;
import java.util.*;

public class transformer{
    public static void main(String[]args){
		try{
			BufferedImage output = ImageIO.read(new File("./volt/output.jpeg"));

			String prefix = "./volt/photos/img";
			String postfix = ".jpg";

			BufferedImage img;

			Random r = new Random();
			int low = 1;
			int high = 100;
			int num = 1;

			//= ImageIO.read(new File("./img1.jpeg"));
	
            num = r.nextInt(high+1-low) + low; 

            img = ImageIO.read(new File(prefix + num + postfix));
            
            num = r.nextInt(4-low) + low;

            for(int i=0;i<output.getWidth();i++){
                for(int j=0;j<output.getHeight();j++){
                    
                    if(num==1)
                        output.setRGB(i,j, output.getRGB(i,j) & img.getRGB(i,j));
                    if(num==2)
                        output.setRGB(i,j, output.getRGB(i,j) | img.getRGB(i,j));
                    if(num==3)
                        output.setRGB(i,j, output.getRGB(i,j) ^ img.getRGB(i,j));
                }
            }

			
			File outputFile = new File("./volt/output.jpeg");
			ImageIO.write(output,"jpeg",outputFile);
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}
}

