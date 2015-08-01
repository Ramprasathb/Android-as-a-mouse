package karthick.ram.MouseServer;

import java.awt.AWTException;
import java.awt.Toolkit;
import java.awt.event.InputEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import java.awt.MouseInfo;
import java.awt.Point;
import java.awt.Robot;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.awt.Rectangle;

public class Server {
	public static BufferedReader bufferedReader;

	public static void main(String args[]) {
		try {
			ServerSocket serverSocket = new ServerSocket(4444);
			Socket socket = serverSocket.accept();
			System.out.println(socket);
			bufferedReader = new BufferedReader(new InputStreamReader(
					socket.getInputStream()));
			Robot robot = new Robot();
			//BufferedImage image = new Robot().createScreenCapture(new Rectangle(Toolkit.getDefaultToolkit().getScreenSize()));
			//ImageIO.write(image, "png", new File("F:/screenshot.png"));
			int currentX, currenty, prevX = 0, prevY = 0;
			robot.mouseMove(0, 0);
			int prex=1,prey=1,slope=0,x3=0,y3=0;
			currentX = 2;
			currenty = 2;
			String  coordinates[] = new String[2];
			while (true) {
				String recieved = bufferedReader.readLine();
				// System.out.println(recieved);
				coordinates = recieved.split(":");
				int x = Integer.parseInt(coordinates[0]);
				int y = (Integer.parseInt(coordinates[1]));
				if(x>1220){
				Robot r = new Robot();
			    //r.mouseMove(35,35);
			    r.mousePress( InputEvent.BUTTON1_MASK );
			    r.mouseRelease( InputEvent.BUTTON1_MASK );
				}
				if(x>1220 && y>500)
				{
					Robot r = new Robot();
				   // r.mouseMove(35,35);
				    r.mousePress( InputEvent.BUTTON3_MASK );
				    r.mouseRelease( InputEvent.BUTTON3_MASK );
				}
			/*	x3=(currentX-prex);
						y3=(currenty-prey);
				if(y3!=0)
					slope=x3/y3;
				System.out.println("slope:" +slope);*/
				
				prex=x; prey=y;
				System.out.println("Phone X : " + x + " Phone Y: " + y);

				Point current = MouseInfo.getPointerInfo().getLocation();
				System.out.println("Previous Phone X : " + prex + " Previous Phone Y: "+ prey);
				System.out.println("comp X : " + current.x + " comp Y: "+ current.y);
				/*if (prex > x)

				{
					System.out.println("-x");
					x = -x;
				}

				if (prey > y) {
					System.out.println("-y");
					y = -y;
			} */
                
				System.out.println("Calibarated X : " + x + " Calibarated Y: "						+ y);
	
				
              /*  int diffx=Math.abs(x-prex);
                int diffy=Math.abs(y-prey);
				
                if(x<prex) diffx*=-1;
                if(x<prey) diffy*=-1;*/
                
                currentX =( prex + (x)) ;                               
				currenty =(prey+(y));

	//			prevX =currentX;
		//		prevY =currenty;
	
				prex=(int)Math.abs(x);
				prey=(int)Math.abs(y);
//				
				System.out.println("Current X : " + currentX + " Current Y: "
						+ currenty);

				
				System.out
						.println("---------------------------------------------------------");
				if(x<1220 && y<500)
				robot.mouseMove(currentX, currenty);

			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (AWTException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
