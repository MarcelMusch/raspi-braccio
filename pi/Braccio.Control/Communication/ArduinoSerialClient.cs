using System.IO.Ports;

namespace Braccio.Control.Communication;

public sealed class ArduinoSerialClient : IDisposable
{
    private readonly SerialPort _port;

    public ArduinoSerialClient(string portName, int baudRate = 115200)
    {
        _port = new SerialPort(portName, baudRate)
        {
            NewLine = "\n",
            ReadTimeout = 1000,
            WriteTimeout = 1000
        };
    }

    public void Open()
    {
        if (!_port.IsOpen)
        {
            _port.Open();
            Console.WriteLine($"Connected to Arduino on {_port.PortName}");
        }
    }

    public void SendLine(string command) => _port.WriteLine(command);

    public string? TryReadLine()
    {
        try { return _port.ReadLine(); }
        catch (TimeoutException) { return null; }
    }

    public void Dispose()
    {
        if (_port.IsOpen) _port.Close();
        _port.Dispose();
    }
}
