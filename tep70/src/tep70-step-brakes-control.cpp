#include    "tep70.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70::stepBrakesControl(double t, double dt)
{
    // Блокировочное устройство
    brake_lock->setFLpressure(main_reservoir->getPressure());
    brake_lock->setBPpressure(brakepipe->getPressure());
    brake_lock->setBCpressure(bc_switch_valve->getPressure1());
    brake_lock->setCraneFLflow(brake_crane->getFLflow() + loco_crane->getFLflow());
    brake_lock->setCraneBPflow(brake_crane->getBPflow());
    brake_lock->setCraneBCflow(loco_crane->getBCflow());
    brake_lock->setControl(keys);
    brake_lock->step(t, dt);

    // Поездной кран машиниста
    brake_crane->setFLpressure(brake_lock->getCraneFLpressure());
    brake_crane->setBPpressure(brake_lock->getCraneBPpressure());

    // Управляем краном, учитывая возможное наличие внешнего пульта
    if (control_signals.analogSignal[CS_BRAKE_CRANE].is_active)
    {
        int brake_crane_pos = static_cast<int>(control_signals.analogSignal[CS_BRAKE_CRANE].cur_value);
        brake_crane->setHandlePosition(brake_crane_pos);
    }
    else
    {
        brake_crane->setControl(keys);
    }

    brake_crane->step(t, dt);

    // Кран вспомогательного тормоза
    loco_crane->setFLpressure(brake_lock->getCraneFLpressure());
    loco_crane->setBCpressure(brake_lock->getCraneBCpressure());
    loco_crane->setILpressure(0.0);

    // Управляем, с учетом возможного наличия пульта
    if (control_signals.analogSignal[CS_LOCO_CRANE].is_active)
    {
        double pos = control_signals.analogSignal[CS_LOCO_CRANE].cur_value;
        loco_crane->setHandlePosition(pos);
    }
    else
    {
        loco_crane->setControl(keys);
    }

    loco_crane->step(t, dt);

    // Переключательный клапан ЗПК
    // Первый вход клапана моделирует магистраль тормозных цилиндров
    // Второй вход клапана моделирует резервуар стабильности (ложный ТЦ)
    // Выход клапана подключен через тройник к повторителям давления тележек
    double bc_flow1 = 0.0;
    bc_flow1 += brake_lock->getBCflow();

    anglecock_bc_fwd->setHoseFlow(hose_bc_fwd->getFlow());
    bc_flow1 += anglecock_bc_fwd->getFlowToPipe();

    anglecock_bc_bwd->setHoseFlow(hose_bc_bwd->getFlow());
    bc_flow1 += anglecock_bc_bwd->getFlowToPipe();

    bc_switch_valve->setInputFlow1(bc_flow1);
    bc_switch_valve->setInputFlow2(electro_air_dist->getBCflow());
    bc_switch_valve->setOutputPressure(bc_splitter->getInputPressure());
    bc_switch_valve->step(t, dt);

    // Тройник
    bc_splitter->setInputFlow(bc_switch_valve->getOutputFlow());
    bc_splitter->setPipePressure1(bc_pressure_relay[TROLLEY_FWD]->getControlPressure());
    bc_splitter->setPipePressure2(bc_pressure_relay[TROLLEY_BWD]->getControlPressure());
    bc_splitter->step(t, dt);

    // Повторительное реле давления №304 передней тележки
    bc_pressure_relay[TROLLEY_FWD]->setFLpressure(main_reservoir->getPressure());
    bc_pressure_relay[TROLLEY_FWD]->setControlFlow(bc_splitter->getPipeFlow1());
    bc_pressure_relay[TROLLEY_FWD]->setPipePressure(brake_mech[TROLLEY_FWD]->getBCpressure());
    bc_pressure_relay[TROLLEY_FWD]->step(t, dt);
    // Повторительное реле давления №304 задней тележки
    bc_pressure_relay[TROLLEY_BWD]->setFLpressure(main_reservoir->getPressure());
    bc_pressure_relay[TROLLEY_BWD]->setControlFlow(bc_splitter->getPipeFlow2());
    bc_pressure_relay[TROLLEY_BWD]->setPipePressure(brake_mech[TROLLEY_BWD]->getBCpressure());
    bc_pressure_relay[TROLLEY_BWD]->step(t, dt);

    // Концевые краны магистрали тормозных цилиндров
    anglecock_bc_fwd->setPipePressure(bc_switch_valve->getPressure1());
    //anglecock_bc_fwd->setControl(keys);
    anglecock_bc_fwd->step(t, dt);

    anglecock_bc_bwd->setPipePressure(bc_switch_valve->getPressure1());
    //anglecock_bc_bwd->setControl(keys);
    anglecock_bc_bwd->step(t, dt);

    // Рукава магистрали тормозных цилиндров
    hose_bc_fwd->setPressure(anglecock_bc_fwd->getPressureToHose());
    hose_bc_fwd->setFlowCoeff(anglecock_bc_fwd->getFlowCoeff());
    hose_bc_fwd->setCoord(train_coord + dir * orient * (length / 2.0 - anglecock_bc_fwd->getShiftCoord()));
    hose_bc_fwd->setShiftSide(anglecock_bc_fwd->getShiftSide());
    //hose_bc_fwd->setControl(keys);
    hose_bc_fwd->step(t, dt);

    hose_bc_bwd->setPressure(anglecock_bc_bwd->getPressureToHose());
    hose_bc_bwd->setFlowCoeff(anglecock_bc_bwd->getFlowCoeff());
    hose_bc_bwd->setCoord(train_coord - dir * orient * (length / 2.0 - anglecock_bc_bwd->getShiftCoord()));
    hose_bc_bwd->setShiftSide(anglecock_bc_bwd->getShiftSide());
    //hose_bc_bwd->setControl(keys);
    hose_bc_bwd->step(t, dt);
}
