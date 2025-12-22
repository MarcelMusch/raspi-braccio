using System;
using System.Threading;
using Braccio.Control.Communication;
using Microsoft.Extensions.Configuration;

namespace Braccio.Control.App
{
    public class AppHost
    {
        private readonly object _serialSync = new();

        private enum State
        {
            Idle,
            Moving,
            Error,
            Homing,
            Shutdown,
            Starting
        }

        // Current state of the application: Start with Homing
        private State _currentState = State.Starting;
        private readonly IArduinoClient _arduino;
        private readonly IConfiguration _configuration;

        public AppHost(IConfiguration configuration)
        {
            _configuration = configuration;
            var port = _configuration["ArduinoPort"] ?? "/dev/ttyUSB0";
            var baudRate = int.TryParse(_configuration["ArduinoBaudRate"], out var rate) ? rate : 115200;
            _arduino = new ArduinoSerialClient(port, baudRate);
        }

        private static string TrimResponse(string response)
        {
            return response.Trim();
        }

        public void WaitForRobotReady()
        {
            Console.WriteLine("Waiting for robot to be ready...");

            var response = AwaitResponse(
                match: line => line == "BRACCIO_READY" || line.StartsWith("ERR"),
                timeoutMilliseconds: 20000
            );

            if (response.StartsWith("ERR"))
                throw new Exception($"Home failed: {response}");

        }

        private string AwaitResponse(Func<string, bool> match, int timeoutMilliseconds = 2000, Action<string>? onIgnoredLine = null)
        {
            var start = DateTime.UtcNow;

            while ((DateTime.UtcNow - start).TotalMilliseconds < timeoutMilliseconds)
            {
                var line = _arduino.TryReadLine();
                if (string.IsNullOrWhiteSpace(line))
                {
                    Thread.Sleep(5);
                    continue;
                }

                line = TrimResponse(line);

                if (match(line))
                    return line;

                onIgnoredLine?.Invoke(line);
                // otherwise ignore and keep reading
            }

            throw new TimeoutException("Timeout waiting for matching serial response.");
        }

        public void InitializeRobot()
        {
            Console.WriteLine("Initializing robot...");

            try
            {
                _arduino.Open();
                WaitForRobotReady();
                SendHomeCommand();
                Console.WriteLine("Robot initialized and homed successfully.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to initialize robot: {ex.Message}");
                throw;
            }
        }

        public void Run()
        {
            Console.WriteLine("AppHost running. Waiting for API calls...");

            // Keep the application alive to handle REST API calls
            while (true)
            {
                Thread.Sleep(1000); // Prevent busy-waiting
            }
        }

        public string GetStateString()
        {
            return _currentState.ToString().ToUpper();
        }

        public string StopRobot()
        {
            _arduino.SendLine("STOP");
            _currentState = State.Idle;

            var response = AwaitResponse(
                match: line => line == "OK" || line.StartsWith("ERR"),
                timeoutMilliseconds: 5000
            );

            if (response.StartsWith("ERR"))
                throw new Exception($"Home failed: {response}");

            return TrimResponse(response) ?? "No response";
        }

        public string Reset()
        {
            // we read multiple lines to clear any buffered messages
            for (int i = 0; i < 10; i++)
            {
                Thread.Sleep(10);
                _arduino.TryReadLine();
            }

            _arduino.SendLine("RESET");

            var response = AwaitResponse(
                        match: line => line == "OK" || line.StartsWith("ERR"),
                        timeoutMilliseconds: 5000
                    );

            if (response.StartsWith("ERR"))
                throw new Exception($"Home failed: {response}");


            _currentState = State.Idle;
            return TrimResponse(response) ?? "No response";
        }

        public string SendSetJointPosCommand(string JointComposition, int stepDelay, int j1, int j2, int j3, int j4, int j5, int j6)
        {
            _currentState = State.Moving;
            _arduino.SendLine($"SET_JOINTS JC={JointComposition} T={stepDelay} J1={j1} J2={j2} J3={j3} J4={j4} J5={j5} J6={j6}");
            var response = AwaitResponse(
                        match: line => line == "OK" || line.StartsWith("ERR"),
                        timeoutMilliseconds: 20000
                    );

            if (response.StartsWith("ERR"))
                throw new Exception($"Home failed: {response}");

            _currentState = State.Idle;
            return TrimResponse(response) ?? "No response";
        }

        // Update GetCurrentPosition to return an object with joint positions
        public (int J1, int J2, int J3, int J4, int J5, int J6) GetCurrentPosition()
        {
            lock (_serialSync)
            {
                _arduino.SendLine("GET_CURRENT_POSITION");

                var response = AwaitResponse(
                    match: line => line.StartsWith("CP "),   // or StartsWith("CP")
                    timeoutMilliseconds: 2000
                );

                return ParseCp(TrimResponse(response));
            }
        }

        private (int J1, int J2, int J3, int J4, int J5, int J6) ParseCp(string response)
        {
            if (string.IsNullOrWhiteSpace(response))
                throw new Exception("Empty response from Arduino.");

            // Example expected:
            // CP J1=90 J2=45 J3=120 J4=80 J5=10 J6=30
            var parts = response.Split(' ', StringSplitOptions.RemoveEmptyEntries);

            if (parts.Length < 7 || parts[0] != "CP")
                throw new Exception($"Invalid CP response: '{response}'");

            int ReadJoint(string part)
            {
                var kv = part.Split('=');
                if (kv.Length != 2 || !int.TryParse(kv[1], out var value))
                    throw new Exception($"Invalid joint value: '{part}'");

                return value;
            }

            return (
                ReadJoint(parts[1]), // J1
                ReadJoint(parts[2]), // J2
                ReadJoint(parts[3]), // J3
                ReadJoint(parts[4]), // J4
                ReadJoint(parts[5]), // J5
                ReadJoint(parts[6])  // J6
            );
        }


        public string SendHomeCommand()
        {
            _currentState = State.Homing;
            _arduino.SendLine("HOME");

            var response = AwaitResponse(
                        match: line => line == "OK" || line.StartsWith("ERR"),
                        timeoutMilliseconds: 5000
                    );

            if (response.StartsWith("ERR"))
                throw new Exception($"Home failed: {response}");

            _currentState = State.Idle;
            return TrimResponse(response) ?? "No response";
        }

        public string Wave()
        {
            _currentState = State.Moving;
            Console.WriteLine("Emulating wave motion...");

            try
            {
                // Move to initial position
                if (SendHomeCommand() != "OK")
                    throw new Exception("Failed to home the robot before waving.");

                // Perform waving motion
                for (int i = 0; i < 3; i++)
                {
                    if (SendSetJointPosCommand("111111", 10, 95, 90, 90, 0, 0, 40) != "OK")
                        throw new Exception("Failed to send wave move command.");
                    Thread.Sleep(500);
                    if (SendSetJointPosCommand("111111", 10, 95, 90, 90, 0, 50, 40) != "OK")
                        throw new Exception("Failed to send wave move command.");
                    Thread.Sleep(500);
                }

                Console.WriteLine("Wave motion completed.");
                SendHomeCommand();
                _currentState = State.Idle;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to emulate wave motion: {ex.Message}");
                _currentState = State.Error;
                return $"Error: {ex.Message}";
            }
            return "Wave command executed.";
        }
    }

}