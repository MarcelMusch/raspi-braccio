using Microsoft.AspNetCore.Mvc;
using Braccio.Control.App;

[ApiController]
[Route("api/robot")]
public class RobotController : ControllerBase
{
    private readonly AppHost _appHost;
    private const int DefaultDuration = 1000; // Define a default duration (in milliseconds)

    public RobotController(AppHost appHost)
    {
        _appHost = appHost;
    }

    [HttpPost("set-joint-positions")]
    public IActionResult SetJointPositions([FromBody] MoveCommand command)
    {
        Console.WriteLine($"Received Move Command: JC={command.JC}, T={command.T}, J1={command.J1}, J2={command.J2}, J3={command.J3}, J4={command.J4}, J5={command.J5}, J6={command.J6}");
        var status = _appHost.SendSetJointPosCommand(
            command.JC,
            command.T,
            command.J1 ?? -1,
            command.J2 ?? -1,
            command.J3 ?? -1,
            command.J4 ?? -1,
            command.J5 ?? -1,
            command.J6 ?? -1
        );
        return Ok(new { success = true, status });
    }

    [HttpGet("current-position")]
    public IActionResult GetCurrentPosition()
    {
        var (J1, J2, J3, J4, J5, J6) = _appHost.GetCurrentPosition();
        return Ok(new { success = true, position = new { J1, J2, J3, J4, J5, J6 } });
    }

    [HttpGet("home")]
    public IActionResult Home()
    {
        var response = _appHost.SendHomeCommand();
        return Ok(new { success = true, response });
    }

    [HttpGet("status")]
    public IActionResult Status()
    {
        var state = _appHost.GetStateString();
        return Ok(new { success = true, state });
    }

    [HttpGet("stop")]
    public IActionResult Stop()
    {
        var response = _appHost.StopRobot();
        return Ok(new { success = true, response });
    }

    [HttpGet("reset")]
    public IActionResult Reset()
    {
        var response = _appHost.Reset();
        return Ok(new { success = true, response });
    }

    [HttpGet("wave")]
    public IActionResult Wave()
    {
        var response =_appHost.Wave();
        return Ok(new { success = true, message = response });
    }
}

public class MoveCommand
{
    required public string JC { get; set; }  // Joint Composition represents which joints to move
    required public int T { get; set; } // STEP Delay represents the time between movements
    public int? J1 { get; set; }
    public int? J2 { get; set; }
    public int? J3 { get; set; }
    public int? J4 { get; set; }
    public int? J5 { get; set; }
    public int? J6 { get; set; }
}