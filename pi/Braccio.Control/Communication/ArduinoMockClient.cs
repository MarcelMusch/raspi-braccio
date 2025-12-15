using System.Collections.Concurrent;

namespace Braccio.Control.Communication;

public sealed class ArduinoMockClient : IArduinoClient
{
    private readonly ConcurrentQueue<string> _rx = new();
    private bool _open;

    public void Open()
    {
        _open = true;
        _rx.Enqueue("BRACCIO_READY");
    }

    public void SendLine(string command)
    {
        if (!_open) throw new InvalidOperationException("Mock client not opened.");

        command = command.Trim();

        if (command.Equals("PING", StringComparison.OrdinalIgnoreCase))
            _rx.Enqueue("ACK:PING");
        else
            _rx.Enqueue($"ACK:{command}");
    }

    public string? TryReadLine()
    {
        return _rx.TryDequeue(out var msg) ? msg : null;
    }

    public void Dispose() { }
}
