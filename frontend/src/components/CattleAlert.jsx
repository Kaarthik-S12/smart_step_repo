import React from "react";
import "./CattleAlert.css";

function CattleAlert({ id, sensingHours, restingHours }) {
  const restingRatio = ((restingHours / sensingHours) * 100).toFixed(1);

  let alertClass = "alert-item";
  if (restingRatio > 74) {
    alertClass += " red-alert";
  } else if (restingRatio > 50) {
    alertClass += " highlighted";
  }

  return (
    <div className={alertClass}>
      {restingRatio > 74 && <div className="alert-icon"> ! </div>}
      {restingRatio > 50 && restingRatio <= 74 && <div className="warning-icon">⚠️</div>}
      <h3 className="id-heading">Cattle ID: {id}</h3>

      <div className="time-boxes">
        <div className="time-card sensing">
          <p>Sensing Time</p>
          <h4>{sensingHours} sec</h4>
        </div>
        <div className="time-card resting">
          <p>Resting Time</p>
          <h4>{restingHours} sec</h4>
        </div>
      </div>
    </div>
  );



  
}

export default CattleAlert;