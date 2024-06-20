const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT = 5000;

app.use(cors());
app.use(bodyParser.json());

app.post('/execute', (req, res) => {
  const { numProcesses, arrivalTimes, burstTimes, priorities, timeQuantum } = req.body;


  console.log('Received data:', req.body);

  const inputData = `${numProcesses}\n${arrivalTimes.join(' ')}\n${burstTimes.join(' ')}\n${priorities.join(' ')}\n${timeQuantum}\n`;

 
  console.log('Input data for C++ program:', inputData);


  fs.writeFileSync('input.txt', inputData);

  exec(`g++ -o a.out your_script.cpp`, (compileError, compileStdout, compileStderr) => {
    if (compileError) {
      console.error('Compilation error:', compileStderr);
      return res.status(500).json({ output: `Compilation error: ${compileStderr}` });
    }

    console.log('Compilation successful');

    
    exec(`a.out < input.txt`, (execError, execStdout, execStderr) => {
      if (execError) {
        console.error('Execution error:', execStderr);
        return res.status(500).json({ output: `Execution error: ${execStderr}` });
      }

      console.log('Execution output:', execStdout);
      res.json({ output: execStdout });
    });
  });
});

app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
