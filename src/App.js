import React, { useState } from 'react';
import './App.css';

function App() {
  const [numProcesses, setNumProcesses] = useState('');
  const [processInput, setProcessInput] = useState('');
  const [timeQuantum, setTimeQuantum] = useState('');
  const [processes, setProcesses] = useState([]);
  const [output, setOutput] = useState('');
  const [error, setError] = useState('');

  const handleAddProcess = (event) => {
    event.preventDefault();

    if (processes.length >= 10) {
      setError('Number of processes cannot exceed 10.');
      return;
    }

    const processDetails = processInput.split(',').map(item => parseFloat(item.trim()));

    if (processDetails.length !== 3 || processDetails.some(item => isNaN(item) || item < 0)) {
      setError('Invalid input format. Please enter arrival time, burst time, and priority separated by commas.');
      return;
    }

    const [arrivalTime, burstTime, priority] = processDetails;

    const newProcess = {
      arrivalTime,
      burstTime,
      priority,
    };

    setProcesses([...processes, newProcess]);
    setProcessInput('');
    setError('');
  };

  const handleSubmit = async (event) => {
    event.preventDefault();

    if (processes.length === 0) {
      setError('Please add at least one process.');
      return;
    }

    const timeQuantumFloat = parseFloat(timeQuantum.trim());

    if (isNaN(timeQuantumFloat) || timeQuantumFloat <= 0) {
      setError('Time quantum must be a positive number.');
      return;
    }

    setError('');

    try {
      const response = await fetch('http://localhost:5000/execute', { 
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          numProcesses: processes.length,
          arrivalTimes: processes.map(p => p.arrivalTime),
          burstTimes: processes.map(p => p.burstTime),
          priorities: processes.map(p => p.priority),
          timeQuantum: timeQuantumFloat
        }),
      });

      if (!response.ok) {
        throw new Error('Network response was not ok');
      }

      const data = await response.json();
      setOutput(data.output);
    } catch (error) {
      setError(`Fetch error: ${error.message}`);
    }
  };

  const handleReset = () => {
    setNumProcesses('');
    setProcessInput('');
    setTimeQuantum('');
    setProcesses([]);
    setOutput('');
    setError('');
  };

  return (
    <div className="App">
      <h1 className="heading">CPU SCHEDULING PROGRAM</h1>
      <div className="content">
        <div className="input">
          <h2>INPUT</h2>
          <form onSubmit={handleAddProcess}>
            <div className="form-group">
              <label>Process Details (Arrival Time, Burst Time, Priority separated by commas)</label>
              <input
                type="text"
                value={processInput}
                onChange={(e) => setProcessInput(e.target.value)}
                placeholder="Ex: 0, 5, 2"
              />
            </div>
            <button type="submit" className="btn">Add Process</button>
          </form>
          {error && <div className="error">{error}</div>}
          <table>
            <thead>
              <tr>
                <th>#</th>
                <th>Arrival Time</th>
                <th>Burst Time</th>
                <th>Priority</th>
              </tr>
            </thead>
            <tbody>
              {processes.map((process, index) => (
                <tr key={index}>
                  <td>{index + 1}</td>
                  <td>{process.arrivalTime}</td>
                  <td>{process.burstTime}</td>
                  <td>{process.priority}</td>
                </tr>
              ))}
            </tbody>
          </table>
          <div className="form-group">
            <label>Time Quantum Period</label>
            <input
              type="text"
              value={timeQuantum}
              onChange={(e) => setTimeQuantum(e.target.value)}
              placeholder="Ex: 5"
            />
          </div>
          <div className="button-group">
            <button onClick={handleSubmit} className="btn">Run</button>
            <button onClick={handleReset} className="btn btn-reset">Reset</button>
          </div>
        </div>
        <div className="output">
          <h2>OUTPUT</h2>
          <pre>{output}</pre>
        </div>
      </div>
    </div>
  );
}

export default App;
