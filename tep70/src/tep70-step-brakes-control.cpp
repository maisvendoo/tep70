#include    "tep70.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70::stepBrakesControl(const double& t, const double& dt)
{
    for (size_t cab_idx : {CAB1, CAB2})
    {
        // Блокировочное устройство
        brake_lock[cab_idx]->setFLpressure(main_reservoir->getPressure());
        brake_lock[cab_idx]->setBPpressure(brakepipe->getPressure());
        brake_lock[cab_idx]->setBCpressure(bc_switch_valve->getPressure1());
        brake_lock[cab_idx]->setCraneFLflow(brake_crane[cab_idx]->getFLflow() + loco_crane[cab_idx]->getFLflow());
        brake_lock[cab_idx]->setCraneBPflow(brake_crane[cab_idx]->getBPflow());
        brake_lock[cab_idx]->setCraneBCflow(loco_crane[cab_idx]->getBCflow());
        brake_lock[cab_idx]->step(t, dt);

        // Поездной кран машиниста
        brake_crane[cab_idx]->setFLpressure(brake_lock[cab_idx]->getCraneFLpressure());
        brake_crane[cab_idx]->setBPpressure(brake_lock[cab_idx]->getCraneBPpressure());
        brake_crane[cab_idx]->step(t, dt);

        // Кран вспомогательного тормоза
        loco_crane[cab_idx]->setFLpressure(brake_lock[cab_idx]->getCraneFLpressure());
        loco_crane[cab_idx]->setBCpressure(brake_lock[cab_idx]->getCraneBCpressure());
        loco_crane[cab_idx]->setILpressure(0.0);
        loco_crane[cab_idx]->step(t, dt);
    }

    double bc_flow1 = 0.0;
    bc_flow1 += brake_lock[CAB1]->getBCflow();
    bc_flow1 += brake_lock[CAB2]->getBCflow();

    anglecock_bc_fwd->setHoseFlow(hose_bc_fwd->getFlow());
    bc_flow1 += anglecock_bc_fwd->getFlowToPipe();

    anglecock_bc_bwd->setHoseFlow(hose_bc_bwd->getFlow());
    bc_flow1 += anglecock_bc_bwd->getFlowToPipe();

    // Редуктор РЕД1 подключен к питательной макгистрали
    redc1->setInputPressure(main_reservoir->getPressure());
    redc1->setOutPressure(rs3->getPressure());
    redc1->step(t, dt);

    // Резервуар РС3 подключен к редуктору РЕД1
    double rs3_flow = 0.0;

    // Расход от редуктора идет через обратный клапан
    rs3_flow += pf(redc1->getOutFlow());
    // Расход воздуха в РЕД3 идет сквозной
    rs3_flow += redc3->getInputFlow() + redc_emrg->getInputFlow() + bc_pressure_relay[TROLLEY_FWD]->getFLflow();

    rs3->setFlow(rs3_flow);
    rs3->step(t, dt);

    // Редуктор РЕД2 подключен к питательной макгистрали
    redc2->setInputPressure(main_reservoir->getPressure());
    redc2->setOutPressure(rs4->getPressure());
    redc2->step(t, dt);

    // Резервуар РС4 подключен к редуктору РЕД2
    double rs4_flow = 0.0;

    // Расход от редуктора идет через обратный клапан
    rs4_flow += pf(redc2->getOutFlow());
    // Расход воздуха сквозной от реле давления второй тележки
    rs4_flow += bc_pressure_relay[TROLLEY_BWD]->getFLflow();

    rs4->setFlow(rs3_flow);
    rs4->step(t, dt);

    // Редуктор РЕД3 подключен к РС3
    redc3->setInputPressure(rs3->getPressure());
    redc3->setOutPressure(vt2->getInputPressure());
    redc3->step(t, dt);

    // Вентиль ВТ2 подключен к РЕД3
    vt2->setInputFlow(redc3->getOutFlow());
    vt2->setOutputPressure(pk3->getPressure1());
    vt2->step(t, dt);

    // Фиктивный редуктор 0.6/0.6 подключен к РС3
    redc_emrg->setInputPressure(rs3->getPressure());
    redc_emrg->setOutPressure(vt3->getInputPressure());
    redc_emrg->step(t, dt);

    // Вентиль ВТ3 подключен к фиктивному редуктору 0.6/0.6
    vt3->setInputFlow(redc_emrg->getOutFlow());
    vt3->setOutputPressure(pk3->getPressure2());
    vt3->step(t, dt);

    // Переключательный клапан ПК3 коммутирует выходы вентилей ВТ2 и ВТ3
    pk3->setInputFlow1(vt2->getOutputFlow());
    pk3->setInputFlow2(vt3->getOutputFlow());
    pk3->setOutputPressure(pk4->getPressure2());
    pk3->step(t, dt);

    // Переключательный клапан ПК4: коммутирует поток воздуха на вентиль ВТ1
    // между ВР/ЭВР и пневматической цепью дотормаживания

    // К первому входу подключен ЭВР305
    pk4->setInputFlow1(electro_air_dist->getBCflow());
    // Ко второму входу пока ничего не подключили!
    pk4->setInputFlow2(pk3->getOutputFlow());
    // Противодавление на выходе от входа ВТ1
    pk4->setOutputPressure(vt1->getInputPressure());
    pk4->step(t, dt);

    // ВТ1 — вентиль отпуска тормозов при работе ЭДТ
    vt1->setInputFlow(pk4->getOutputFlow());
    vt1->setOutputPressure(bc_switch_valve->getPressure2());
    vt1->step(t, dt);

    // Переключательный клапан ЗПК
    // Первый вход клапана моделирует магистраль тормозных цилиндров
    // Второй вход клапана моделирует резервуар стабильности (ложный ТЦ)
    // Выход клапана подключен через тройник к повторителям давления тележек
    bc_switch_valve->setInputFlow1(bc_flow1);
    bc_switch_valve->setInputFlow2(vt1->getOutputFlow());
    bc_switch_valve->setOutputPressure(bc_splitter->getInputPressure());
    bc_switch_valve->step(t, dt);

    // Тройник
    bc_splitter->setInputFlow(bc_switch_valve->getOutputFlow());
    bc_splitter->setPipePressure1(bc_pressure_relay[TROLLEY_FWD]->getControlPressure());
    bc_splitter->setPipePressure2(bc_pressure_relay[TROLLEY_BWD]->getControlPressure());
    bc_splitter->step(t, dt);

    // Повторительное реле давления №304 передней тележки
    bc_pressure_relay[TROLLEY_FWD]->setFLpressure(rs3->getPressure());
    bc_pressure_relay[TROLLEY_FWD]->setControlFlow(bc_splitter->getPipeFlow1());
    bc_pressure_relay[TROLLEY_FWD]->setPipePressure(brake_mech[TROLLEY_FWD]->getBCpressure());
    bc_pressure_relay[TROLLEY_FWD]->step(t, dt);
    // Повторительное реле давления №304 задней тележки
    bc_pressure_relay[TROLLEY_BWD]->setFLpressure(rs4->getPressure());
    bc_pressure_relay[TROLLEY_BWD]->setControlFlow(bc_splitter->getPipeFlow2());
    bc_pressure_relay[TROLLEY_BWD]->setPipePressure(brake_mech[TROLLEY_BWD]->getBCpressure());
    bc_pressure_relay[TROLLEY_BWD]->step(t, dt);

    // Концевые краны магистрали тормозных цилиндров
    anglecock_bc_fwd->setPipePressure(bc_switch_valve->getPressure1());
    anglecock_bc_fwd->step(t, dt);

    anglecock_bc_bwd->setPipePressure(bc_switch_valve->getPressure1());
    anglecock_bc_bwd->step(t, dt);

    // Рукава магистрали тормозных цилиндров
    hose_bc_fwd->setPressure(anglecock_bc_fwd->getPressureToHose());
    hose_bc_fwd->setFlowCoeff(anglecock_bc_fwd->getFlowCoeff());
    hose_bc_fwd->setCoord(train_coord + dir * (length / 2.0 - anglecock_bc_fwd->getShiftCoord()));
    hose_bc_fwd->setShiftSide(anglecock_bc_fwd->getShiftSide());
    hose_bc_fwd->step(t, dt);

    hose_bc_bwd->setPressure(anglecock_bc_bwd->getPressureToHose());
    hose_bc_bwd->setFlowCoeff(anglecock_bc_bwd->getFlowCoeff());
    hose_bc_bwd->setCoord(train_coord - dir * (length / 2.0 - anglecock_bc_bwd->getShiftCoord()));
    hose_bc_bwd->setShiftSide(anglecock_bc_bwd->getShiftSide());
    hose_bc_bwd->step(t, dt);
}
