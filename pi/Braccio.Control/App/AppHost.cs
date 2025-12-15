namespace Braccio.Control.App;

public class AppHost
{
    public void Run()
    {
        Console.WriteLine("AppHost running.");
        Console.WriteLine("Press Ctrl+C to exit.");

        // TODO:
        // - Load config
        // - Initialize ArduinoSerialClient
        // - Start state machine / main loop

        while (true)
        {
            Thread.Sleep(1000);
        }
    }
}
