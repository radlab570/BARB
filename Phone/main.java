import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class main {
	public static void main(String[] args) {
		DatagramSocket socket = null;
        String targetIP = "127.0.0.1"; //138.47.102.255
		int port = 10000;
	    int NUM_FLOATS = 7;
		
        try {
			//create socket
			socket = new DatagramSocket(10001);
			
			//create message and convert to byte array
			float[] nums = {0f, 0f, 0f, 0f, -0.99f, -0.99f, -0.99f};

			// Convert floats to byte array
            ByteBuffer bufferData = ByteBuffer.allocate(NUM_FLOATS * 4);
			bufferData.order(ByteOrder.LITTLE_ENDIAN); // Set byte order to little-endian
            for (float value : nums) {
                bufferData.putFloat(value);
            }

            byte[] buffer = bufferData.array();
			
			//configure address
			InetAddress address = InetAddress.getByName(targetIP);
			
			//create packet
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length, address, port);
			
			//send packet
			boolean increase = true;
			
			while (true) {
				
				if (increase) {
					for (int i = 0; i<nums.length; i++) {
						nums[i] = nums[i] + 0.001f;
					}
					if (nums[0] > 0.97) {
						increase = false;
					}
				} else {
					for (int j = 0; j<nums.length; j++) {
						nums[j] = nums[j] - 0.001f;
					}
					if (nums[0] < -0.97) {
						increase = true;
					}
				}
				bufferData = ByteBuffer.allocate(NUM_FLOATS * 4);
				bufferData.order(ByteOrder.LITTLE_ENDIAN); // Set byte order to little-endian
				for (float value : nums) {
					bufferData.putFloat(value);
				}
				buffer = bufferData.array();
				packet = new DatagramPacket(buffer, buffer.length, address, port);
				socket.send(packet);
				/*bufferData.rewind();
				for (int i=0; i<NUM_FLOATS; i++){
					System.out.println(bufferData.getFloat());
				}*/
			}
			//System.out.println("Packet sent");
			//System.out.println(buffer.length);
			
			//close socket
			//socket.close();
		} catch (Exception e) {
			System.out.println(e);
		}
	}
}