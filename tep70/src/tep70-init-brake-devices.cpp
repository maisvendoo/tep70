#include    "tep70.h"

#include    "filesystem.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70::initBrakeDevices(double p0, double pBP, double pFL)
{
    charge_press = p0;

    // Загрузка состояния тормозного оборудования из собственного конфига
    FileSystem &fs = FileSystem::getInstance();
    QString custom_cfg_dir(fs.getVehiclesDir().c_str());
    custom_cfg_dir += QDir::separator() + config_dir;
    load_brakes_config(custom_cfg_dir + QDir::separator() + "brakes-init.xml");

    // Инициализация давления в приборах управления тормозами
    for (size_t cab_idx : {CAB1, CAB2})
    {
        brake_lock[cab_idx]->init(pBP, pFL);

        brake_crane[cab_idx]->init(pBP, pFL);
        brake_crane[cab_idx]->setChargePressure(p0);

        loco_crane[cab_idx]->init(pBP, pFL);

        epk[cab_idx]->init(pBP, pFL);
    }

    // Инициализация давления в питательной магистрали
    main_reservoir->setY(0, pFL);
    anglecock_fl_fwd->setPipePressure(pFL);
    anglecock_fl_bwd->setPipePressure(pFL);
    hose_fl_fwd->setPressure(pFL);
    hose_fl_bwd->setPressure(pFL);

    // Инициализация давления в тормозной магистрали
    brakepipe->setY(0, pBP);
    anglecock_bp_fwd->setPipePressure(pBP);
    anglecock_bp_bwd->setPipePressure(pBP);
    hose_bp_fwd->setPressure(pBP);
    hose_bp_bwd->setPressure(pBP);

    air_dist->init(pBP, pFL);
    electro_air_dist->init(pBP, pFL);

    supply_reservoir->setY(0, pBP);

    // Состояние рукавов и концевых кранов магистрали тормозных цилиндров
    if (hose_bc_fwd->isLinked())
    {
        hose_bc_fwd->connect();
        anglecock_bc_fwd->open();
    }
    else
    {
        anglecock_bc_fwd->close();
    }

    if (hose_bc_bwd->isLinked())
    {
        hose_bc_bwd->connect();
        anglecock_bc_bwd->open();
    }
    else
    {
        anglecock_bc_bwd->close();
    }

    // Состояние рукавов и концевых кранов питательной магистрали
    if (hose_fl_fwd->isLinked())
    {
        hose_fl_fwd->connect();
        anglecock_fl_fwd->open();
    }
    else
    {
        anglecock_fl_fwd->close();
    }

    if (hose_fl_bwd->isLinked())
    {
        hose_fl_bwd->connect();
        anglecock_fl_bwd->open();
    }
    else
    {
        anglecock_fl_bwd->close();
    }

    // Состояние рукавов и концевых кранов тормозной магистрали
    if (hose_bp_fwd->isLinked())
    {
        hose_bp_fwd->connect();
        anglecock_bp_fwd->open();
    }
    else
    {
        anglecock_bp_fwd->close();
    }

    if (hose_bp_bwd->isLinked())
    {
        hose_bp_bwd->connect();
        anglecock_bp_bwd->open();
    }
    else
    {
        anglecock_bp_bwd->close();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::load_brakes_config(QString path)
{
    CfgReader cfg;

    if (cfg.load(path))
    {
        QString secName = "BrakesState";
        double tmp_dbl;
        int tmp_int;

        tmp_dbl = 1.0e-4;
        if (cfg.getDouble(secName, "MainReservoirLeak", tmp_dbl))
        {
            main_reservoir->setLeakCoeff(tmp_dbl);
        }

        tmp_int = 2;
        if (cfg.getInt(secName, "TrainCranePosCab1", tmp_int))
        {
            brake_crane[CAB1]->setHandlePosition(tmp_int - 1);
        }

        tmp_int = 7;
        if (cfg.getInt(secName, "TrainCranePosCab2", tmp_int))
        {
            brake_crane[CAB2]->setHandlePosition(tmp_int - 1);
        }

        tmp_dbl = 1.0;
        if (cfg.getDouble(secName, "LocoCranePosCab1", tmp_dbl))
        {
            loco_crane[CAB1]->setHandlePosition(tmp_dbl);
        }

        tmp_dbl = 1.0;
        if (cfg.getDouble(secName, "LocoCranePosCab2", tmp_dbl))
        {
            loco_crane[CAB2]->setHandlePosition(tmp_dbl);
        }

        tmp_int = 0;
        if (cfg.getInt(secName, "CombineCranePosCab1", tmp_int))
        {
            brake_lock[CAB1]->setCombineCranePosition(tmp_int);
        }

        tmp_int = -1;
        if (cfg.getInt(secName, "CombineCranePosCab2", tmp_int))
        {
            brake_lock[CAB2]->setCombineCranePosition(tmp_int);
        }

        tmp_int = 1;
        if (cfg.getInt(secName, "BrakeLockDeviceCab1", tmp_int))
        {
            brake_lock[CAB1]->setStateOn(tmp_int);
        }
        // Не допускаем двух рукояток в устройствах блокировки тормозов
        brake_lock[CAB2]->allowLockHandle(!(brake_lock[CAB1]->isLockHandle()));

        tmp_int = 0;
        if (cfg.getInt(secName, "BrakeLockDeviceCab2", tmp_int))
        {
            brake_lock[CAB2]->setStateOn(tmp_int);
        }
        // Не допускаем двух рукояток в устройствах блокировки тормозов
        brake_lock[CAB1]->allowLockHandle(!(brake_lock[CAB2]->isLockHandle()));

        tmp_int = 1;
        if (cfg.getInt(secName, "EPKCab1", tmp_int))
        {
            switch (tmp_int) {
            case 2:
            {
                epk[CAB1]->insertKey(true);
                epk[CAB1]->setKeyOn(true);
                break;
            }
            case 1:
            {
                epk[CAB1]->insertKey(true);
                epk[CAB1]->setKeyOn(false);
                break;
            }
            case 0:
            default:
            {
                epk[CAB1]->insertKey(false);
                break;
            } }
        }
        // Не допускаем двух ключей в электропневматических клапанах автостопа
        epk[CAB2]->allowKey(!(epk[CAB1]->isKey()));

        tmp_int = 0;
        if (cfg.getInt(secName, "EPKCab2", tmp_int))
        {
            switch (tmp_int) {
            case 2:
            {
                epk[CAB2]->insertKey(true);
                epk[CAB2]->setKeyOn(true);
                break;
            }
            case 1:
            {
                epk[CAB2]->insertKey(true);
                epk[CAB2]->setKeyOn(false);
                break;
            }
            case 0:
            default:
            {
                epk[CAB2]->insertKey(false);
                break;
            } }
        }
        // Не допускаем двух ключей в электропневматических клапанах автостопа
        epk[CAB1]->allowKey(!(epk[CAB2]->isKey()));

        tmp_int = 1;
        if (cfg.getInt(secName, "ReversHandleCab1", tmp_int))
        {
            km[CAB1]->insertReversHandle(tmp_int);
        }
        // Не допускаем двух реверсивных рукояток в контроллерах машиниста
        km[CAB2]->allowReversHandle(!(km[CAB1]->isReversHandle()));

        tmp_int = 0;
        if (cfg.getInt(secName, "ReversHandleCab2", tmp_int))
        {
            km[CAB2]->insertReversHandle(tmp_int);
        }
        // Не допускаем двух реверсивных рукояток в контроллерах машиниста
        km[CAB1]->allowReversHandle(!(km[CAB2]->isReversHandle()));
    }
}
