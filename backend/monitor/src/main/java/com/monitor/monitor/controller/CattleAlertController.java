package com.monitor.monitor.controller;
import com.monitor.monitor.model.CattleAlert;
import com.monitor.monitor.service.CattleAlertService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import java.util.List;
@RestController
@RequestMapping("/alerts")
@CrossOrigin  // React frontend, optional
public class CattleAlertController {

    @Autowired
    private CattleAlertService service;

    @GetMapping
    public List<CattleAlert> getAlerts() {
        return service.getAllAlerts();
    }

    @PostMapping
    public CattleAlert saveAlert(@RequestBody CattleAlert alert) {
        return service.saveAlert(alert);
    }

    @DeleteMapping("/clear")
    public void clearProgress() {
        service.clearAlerts();
    }
}