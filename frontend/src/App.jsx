import React, { useEffect, useState } from "react";
import axios from "axios";
import CattleAlert from "./components/CattleAlert";
import "./App.css";

function App() {
  const [alerts, setAlerts] = useState([]);
  const [resetSignal, setResetSignal] = useState(false);
  const fetchAlerts = async () => {
    try {
      const response = await axios.get("http://localhost:8080/alerts");
      setAlerts(response.data);
    } catch (error) {
      console.error("Failed to fetch alerts:", error);
    }
  };

  const clearAlerts = async () => {
    try {
      await axios.delete("http://localhost:8080/alerts/clear");
      setResetSignal((prev) => !prev);
      fetchAlerts();
    } catch (error) {
      console.error("Failed to clear alerts:", error);
    }
  };

  useEffect(() => {
    fetchAlerts();
    const interval = setInterval(fetchAlerts, 5000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div className="container">
       <div className="dashboard-content">
       <div className="dashboard-header">
  <h1 className="dashboard-title">
    <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
      <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M13 7h8m0 0v8m0-8l-8 8-4-4-6 6" />
    </svg>
    Cattle Monitoring Dashboard
  </h1>
  
  <div className="status-indicator">
    <div className="status-item" title="Normal resting patterns">
      <div className="status-color status-normal"></div>
      <span style={{color:"black"}}>Normal</span>
    </div>
    <div className="status-item" title="Slightly abnormal behavior">
      <div className="status-color status-warning"></div>
      <span style={{color:"black"}}>Warning</span>
    </div>
    <div className="status-item" title="Immediate attention required">
      <div className="status-color status-critical"></div>
      <span style={{color:"black"}}>Critical</span>
    </div>

  </div>
</div>

      <div className="alert-box">
        <div className="alert-header">
          <h2>Active Alerts <span className="alert-count">{alerts.length}</span></h2>
          <button className="clear-button" onClick={clearAlerts}>
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M19 7l-.867 12.142A2 2 0 0116.138 21H7.862a2 2 0 01-1.995-1.858L5 7m5 4v6m4-6v6m1-10V4a1 1 0 00-1-1h-4a1 1 0 00-1 1v3M4 7h16" />
            </svg>
            <span>CLEAR ALL</span>
          </button>
        </div>

        <div className="alert-list">
          {alerts.length === 0 ? (
            <div className="empty-state">
              <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={1.5} d="M9.172 16.172a4 4 0 015.656 0M9 10h.01M15 10h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
              </svg>
              <h3>No Active Alerts</h3>
              <p>All cattle are within normal resting patterns</p>
            </div>
          ) : (
            alerts.map((alert) => (
              <CattleAlert
                key={alert.id}
                id={alert.id}
                sensingHours={alert.sensingHours}
                restingHours={alert.restingHours}
                resetSignal={resetSignal}
              />
            ))
          )}
        </div>
      </div>
    </div>
    </div>
  );
}


export default App;