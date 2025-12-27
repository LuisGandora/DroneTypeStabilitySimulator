//meant to be middle wear between main and server, use these commands to set up
//npm init -y
//npm install express cors
const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const app = express();
const port = 3000;

//Allow the front end to access this server
const cors = require('cors');
app.use(cors());

//serve static files
app.use(express.static('.'));

//This is what will solve the simulation stuff with c++
app.get('/run-simulation', (req, res)=>{
    const droneIdx = req.query.droneIdx;
    const windIdx = req.query.windIdx;
    const altitude = req.query.alt;

    console.log(`Running C++ with : Drone=${droneIdx}, Wind=${windIdx}, Alt=${altitude}`);

    exec(`out.exe ${droneIdx} ${windIdx} ${altitude}`, (error,stdout,stderr) =>{ //later replace with the inputs
        if(error)
        {
            console.error(`exec error: ${error}`);
            return res.status(500).json({ error: error.message });
        }
        if(stderr)
        {
            console.error(`stderr: ${stderr}`);
        }

        console.log(`c++ output: ${stdout}`);
        res.json({
            success: true,
            output: stdout
        });

    });


});

app.listen(port, ()=>{
    console.log(`Server running at http://localhost:${port}`);
});

