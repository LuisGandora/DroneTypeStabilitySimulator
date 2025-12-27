
//Get all the selection for names and link them to the drone name 
document.querySelectorAll(".selectName").forEach(button => {
    button.addEventListener("click", (event)=>{
        setDroneOne(event.target.textContent);
    });
});


document.querySelectorAll(".selectWind").forEach(button => {
    button.addEventListener("click", (event)=>{
        setWindSpeed(event.target.id);
    });
});


let droneName = null;
let idx = null;
let altitude=null;


document.getElementById("Altitude").addEventListener("input", (event)=>{
    altitude = event.target.value;
    console.log(altitude)
});

function setDroneOne(name)
{
    //after setting name, display values obtained from .cpp
    droneName = name;
    console.log(droneName);
}


//after setting altitude and wind speed, the simulation should be able to be highlighted
function setWindSpeed(idOfIdx)
{
    idx = idOfIdx;
    console.log(idx);
}

//Now get the run button and check for everything
document.getElementById("SimulationRunner").addEventListener("click", ()=>
{
    if(droneName == null)
    {
        alert("Please select a Drone Name first!");
        return;
    }
    if(idx == null)
    {
        alert("Please select your Wind Speed!");
        return;
    }
    if(altitude == null || altitude <= 0)
    {
        alert("Please input a altitude greater than 0");
        return;
    }

    console.log("Starting simulation");
    /*Execution test*/
    //If Locally: http://localhost:3000/run-simulation?droneIdx=${droneName}&windIdx=${idx}&alt=${altitude}
    //If on netifly for deployment: https://dronestabsimulatorbackendrepo.onrender.com/run-simulation?droneIdx=${droneName}&windIdx=${idx}&alt=${altitude}
    //Need to do this netifly test later because it keeps on bugging, for now run locally via: node server.js
    fetch(`http://localhost:3000/run-simulation?droneIdx=${droneName}&windIdx=${idx}&alt=${altitude}`)
    .then(response => response.json())
    .then(data =>{
        if(data.success)
        {
            console.log("C++ Calc Complete!", data.output);
            window.open('Build/index.html', 'DroneSimulationWindow', 'width=960,height=600');
            
        }
        else{
            alert("simulation failed" + data.error);
        }
    })
    .catch(err =>{
        console.error("Server Error", err);
        alert("Server is not running");
    });
    // const container = document.getElementById("unity-container");
    // container.innerHTML = '<iframe src="Build/index.html" width = "960" height="600" scrolling = "no" allowfullscreen></iframe>';
    // container.scrollIntoView({behavior: 'smooth'});

    
});
