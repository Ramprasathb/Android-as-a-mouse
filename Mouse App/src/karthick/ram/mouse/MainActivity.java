package karthick.ram.mouse;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.R.integer;
import android.os.Bundle;
import android.os.StrictMode;
import android.app.Activity;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends Activity {

	public final static String IP_ADDRESS = "192.168.145.1";
	PrintWriter printWriter;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder()
				.permitAll().build();
		StrictMode.setThreadPolicy(policy);

		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		final TextView textView = (TextView) findViewById(R.id.textView);
		// this is the view on which you will listen for touch events
		try {
			Socket socket = new Socket(IP_ADDRESS, 4444);
			System.out.println(socket);
			 printWriter = new PrintWriter(socket.getOutputStream(),
					true);
		final View touchView = findViewById(R.id.touchView);
	
		
		
			touchView.setOnTouchListener(new View.OnTouchListener() {

				@Override
				public boolean onTouch(View v, MotionEvent event) {
					// TODO Auto-generated method stub
					int xPosition = (int) event.getX();
					int yPosition = (int) event.getY();
					textView.setText("Touch coordinates : "
							+ String.valueOf(xPosition) + "   x   "
							+ (String.valueOf(yPosition)));
					printWriter.write(String.valueOf(xPosition)+":"+String.valueOf(yPosition)+"\n");
					printWriter.flush();

					return true;
				}

			});

		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}