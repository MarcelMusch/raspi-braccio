using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Braccio.Control.App;
using Swashbuckle.AspNetCore.SwaggerGen;

var builder = WebApplication.CreateBuilder(args);

// Add services
builder.Services.AddSingleton<AppHost>(); // Register your AppHost
builder.Services.AddControllers(); // Add controllers for REST API

// Add Swagger services
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

// Update CORS policy to allow all origins

builder.Services.AddCors(options =>
{
    options.AddPolicy("AllowAll", builder =>
    {
        builder.AllowAnyOrigin()
               .AllowAnyHeader()
               .AllowAnyMethod();
    });
});


var app = builder.Build();

// Serve static files (e.g., favicon.ico)
app.UseStaticFiles();

// Initialize the robot
var appHost = app.Services.GetRequiredService<AppHost>();
try
{
    appHost.InitializeRobot();
}
catch (Exception ex)
{
    Console.WriteLine($"Failed to initialize AppHost: {ex.Message}");
    return;
}

// Enable Swagger middleware
if (app.Environment.IsDevelopment() || app.Configuration.GetValue<bool>("EnableSwagger"))
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

// Enable updated CORS middleware
app.UseCors("AllowAll");

// Map controllers
app.MapControllers();

// Start the web application
app.Run();
