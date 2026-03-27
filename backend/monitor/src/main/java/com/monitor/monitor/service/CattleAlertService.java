package com.monitor.monitor.service;
import com.monitor.monitor.model.CattleAlert;
import com.monitor.monitor.repository.CattleAlertRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.util.List;
@Service
public class CattleAlertService {

    @Autowired
    private CattleAlertRepository repository;

    public List<CattleAlert> getAllAlerts() {
        return repository.findAll();
    }

    public CattleAlert saveAlert(CattleAlert alert) {
        return repository.save(alert);
    }

    public void clearAlerts() {
        repository.deleteAll();
    }
}