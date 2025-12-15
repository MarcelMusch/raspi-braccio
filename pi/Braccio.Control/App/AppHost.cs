using Braccio.Control.Communication;

namespace Braccio.Control.App;

public class AppHost
{
    public void Run()
    {
        Console.WriteLine("AppHost running. Press Ctrl+C to exit.");

        // Use MOCK until hardware arrives
        // Later switch to: new ArduinoSerialClient("/dev/ttyACM0", 115200)
        using IArduinoClient arduino = new ArduinoMockClient();

        arduino.Open();

        // Read initial message (if any)
        var hello = arduino.TryReadLine();
        if (hello is not null) Console.WriteLine($"Arduino says: {hello}");

        Console.WriteLine("Sending PING...");
        arduino.SendLine("PING");

        var response = arduino.TryReadLine();
        Console.WriteLine(response is null ? "No response." : $"Arduino says: {response}");

        while (true)
            Thread.Sleep(1000);
    }
}
