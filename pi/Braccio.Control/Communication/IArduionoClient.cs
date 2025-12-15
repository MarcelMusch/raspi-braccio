namespace Braccio.Control.Communication;

public interface IArduinoClient : IDisposable
{
    void Open();
    void SendLine(string command);
    string? TryReadLine();
}
