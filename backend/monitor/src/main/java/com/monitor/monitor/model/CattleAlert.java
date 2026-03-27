package com.monitor.monitor.model;

import jakarta.persistence.*;

@Entity
public class CattleAlert {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private double sensingHours;
    private double restingHours;

    public CattleAlert() {}

    public CattleAlert(Long id, double sensingHours, double restingHours) {
        this.id = id;
        this.sensingHours = sensingHours;
        this.restingHours = restingHours;
    }

    public Long getId() { return id; }
    public double getSensingHours() { return sensingHours; }
    public double getRestingHours() { return restingHours; }

    public void setId(Long id) { this.id = id; }
    public void setSensingHours(double sensingHours) { this.sensingHours = sensingHours; }
    public void setRestingHours(double restingHours) { this.restingHours = restingHours; }
}
