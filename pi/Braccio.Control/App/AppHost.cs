using System;
using System.Threading;
using Braccio.Control.Communication;

namespace Braccio.Control.App
{
    public class AppHost
    {
        private enum State
        {
            Idle,
            Moving,
            Error,
            Homing,
            Shutdown
        }

        // Current state of the application: Start with Homing
        private State _currentState = State.Homing;
        private readonly IArduinoClient _arduino;

        public AppHost()
        {
            _arduino = new ArduinoSerialClient("/dev/ttyUSB0", 115200);
        }

        public void Run()
        {
            Console.WriteLine("AppHost running. Press Ctrl+C to exit.");

            _arduino.Open();

            // Wait for the robot to signal readiness
            WaitForRobotReady();

            while (true)
            {
                switch (_currentState)
                {
                    case State.Idle:
                        HandleIdleState();
                        break;
                    case State.Moving:
                        HandleMovingState();
                        break;
                    case State.Error:
                        HandleErrorState();
                        break;
                    case State.Homing:
                        HandleHomingState();
                        break;
                    case State.Shutdown:
                        HandleShutdownState();
                        return;
                }

                Thread.Sleep(100); // Prevent busy-waiting
            }
        }

        private static string TrimResponse(string response)
        {
            return response.Trim();
        }

        public void WaitForRobotReady()
        {
            Console.WriteLine("Waiting for robot to be ready...");

            while (true)
            {
                var response = _arduino.TryReadLine();

                if (response != null && TrimResponse(response) == "BRACCIO_READY")
                {
                    Console.WriteLine("Robot is ready.");
                    break;
                }

                Console.WriteLine("Still waiting for robot... Retrying in 1 second.");
                Thread.Sleep(1000);
            }
        }

        private string? AwaitResponse(int timeoutMilliseconds = 10000)
        {
            var startTime = DateTime.Now;

            while ((DateTime.Now - startTime).TotalMilliseconds < timeoutMilliseconds)
            {
                var response = _arduino.TryReadLine();
                if (!string.IsNullOrEmpty(response))
                {
                    return TrimResponse(response);
                }

                Thread.Sleep(100); // Avoid busy-waiting
            }

            Console.WriteLine("AwaitResponse: Timeout waiting for response.");
            return null;
        }

        private void HandleIdleState()
        {
            Console.WriteLine("State: Idle");
            Console.WriteLine("Sending PING...");
            _arduino.SendLine("PING");

            var response = AwaitResponse();
            if (response == "ACK:PING")
            {
                Console.WriteLine("PING successful. Transitioning to Moving state.");
                _currentState = State.Moving;
            }
            else
            {
                Console.WriteLine("No response or error. Transitioning to Error state.");
                _currentState = State.Error;
            }
        }

        private void HandleMovingState()
        {
            Console.WriteLine("State: Moving");
            _arduino.SendLine("MOVE J1=90 J2=45 J3=30 J4=0 J5=0 J6=20");

            var response = AwaitResponse();
            if (response == "OK")
            {
                Console.WriteLine("Move successful. Returning to Idle state.");
                _currentState = State.Idle;
            }
            else
            {
                Console.WriteLine("Move failed. Transitioning to Error state.");
                _currentState = State.Error;
            }
        }

        private void HandleErrorState()
        {
            Console.WriteLine("State: Error");
            Console.WriteLine("Resetting error...");
            _arduino.SendLine("RESET");

            var response = AwaitResponse();
            if (response == "OK")
            {
                Console.WriteLine("Error cleared. Returning to Idle state.");
                _currentState = State.Idle;
            }
            else
            {
                Console.WriteLine("Error reset failed. Remaining in Error state.");
            }
        }

        private void HandleHomingState()
        {
            Console.WriteLine("State: Homing");
            _arduino.SendLine("HOME");

            var response = AwaitResponse();
            if (response == "OK")
            {
                Console.WriteLine("Homing successful. Returning to Idle state.");
                _currentState = State.Idle;
            }
            else
            {
                Console.WriteLine("Homing failed. Transitioning to Error state.");
                _currentState = State.Error;
            }
        }

        private void HandleShutdownState()
        {
            Console.WriteLine("State: Shutdown");
            // Example: Perform cleanup
            _arduino.Dispose();
            Console.WriteLine("System shut down.");
        }
    }
}