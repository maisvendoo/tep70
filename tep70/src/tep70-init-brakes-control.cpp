#include    "tep70.h"

#include    <core/load_module.h>

#include    <QDir>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70::initBrakesControl(const QString &modules_dir, const QString &custom_cfg_dir)
{
    for (size_t cab_idx : {CAB1, CAB2})
    {
        // Блокировочное устройство
        brake_lock[cab_idx] = new PneumoBrakeLock();
        brake_lock[cab_idx]->read_config("ubt367m");

        // Поездной кран машиниста
        brake_crane[cab_idx] = LOAD_MODULE(BrakeCrane,
            modules_dir + QDir::separator() + brake_crane_module_name);
        brake_crane[cab_idx]->read_config(brake_crane_config_name);

        // Кран вспомогательного тормоза
        loco_crane[cab_idx] = LOAD_MODULE(LocoCrane,
            modules_dir + QDir::separator() + loco_crane_module_name);
        loco_crane[cab_idx]->read_config(loco_crane_config_name);
    }

    // Переключательный клапан магистрали тормозных цилиндров
    bc_switch_valve = new SwitchingValve();
    bc_switch_valve->read_config("zpk", custom_cfg_dir);

    // Тройник
    bc_splitter = new PneumoSplitter();
    bc_splitter->read_config("pneumo-splitter");

    // Повторительное реле давления
    bc_pressure_relay[TROLLEY_FWD] = new PneumoRelay();
    bc_pressure_relay[TROLLEY_FWD]->read_config("rd304");
    bc_pressure_relay[TROLLEY_BWD] = new PneumoRelay();
    bc_pressure_relay[TROLLEY_BWD]->read_config("rd304");

    // Концевые краны магистрали тормозных цилиндров
    anglecock_bc_fwd = new PneumoAngleCock();
    anglecock_bc_fwd->read_config("pneumo-anglecock-BC");

    anglecock_bc_bwd = new PneumoAngleCock();
    anglecock_bc_bwd->read_config("pneumo-anglecock-BC");

    // Рукава магистрали тормозных цилиндров
    hose_bc_fwd = new PneumoHose();
    hose_bc_fwd->read_config("pneumo-hose-BC");
    forward_connectors.push_back(hose_bc_fwd);

    hose_bc_bwd = new PneumoHose();
    hose_bc_bwd->read_config("pneumo-hose-BC");
    backward_connectors.push_back(hose_bc_bwd);

    // Вентиль отпуска тормозов ВТ1 (нормально открытый)
    vt1 = new ElectroPneumaticValve;
    vt1->read_config("vt1", custom_cfg_dir);

    // Переключательный клапан ПК4
    pk4 = new SwitchingValve;
    pk4->read_config("zpk", custom_cfg_dir);

    // Редуктор РЕД1 на 0.6 МПа
    redc1 = new PneumoReducer;
    redc1->read_config("redc1", custom_cfg_dir);

    rs3 = new Reservoir(0.078);
    rs3->setLeakCoeff(3e-6);

    vt2 = new ElectroPneumaticValve;
    vt2->read_config("vt2", custom_cfg_dir);

    vt3 = new ElectroPneumaticValve;
    vt3->read_config("vt3", custom_cfg_dir);

    // Редуктор РЕД1 на 0.2 МПа
    redc3 = new PneumoReducer;
    redc3->read_config("redc3", custom_cfg_dir);

    // Переключательный клапан ПК3
    pk3 = new SwitchingValve;
    pk3->read_config("zpk", custom_cfg_dir);

    // Редуктор давления ЭТ от вентиля ВТ3. В схеме теплвоза его нет
    redc_emrg = new PneumoReducer;
    redc_emrg->read_config("redc_emrg", custom_cfg_dir);
}
