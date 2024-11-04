#include    "tep70bs.h"

#include    "filesystem.h"

#include    <CfgReader.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TEP70BS::TEP70BS() : Vehicle()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TEP70BS::~TEP70BS()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::initialization()
{
    FileSystem &fs = FileSystem::getInstance();
    QString modules_dir = QString(fs.getModulesDir().c_str());
    QString custom_cfg_dir(fs.getVehiclesDir().c_str());
    custom_cfg_dir += fs.separator() + config_dir;

    initCouplings(modules_dir, custom_cfg_dir);

    initCabineControls(modules_dir, custom_cfg_dir);

    initControlCircuit(modules_dir, custom_cfg_dir);

    initFuelSystem(modules_dir, custom_cfg_dir);

    initDisel(modules_dir, custom_cfg_dir);

    initOilSystem(modules_dir, custom_cfg_dir);

    initPneumoSupply(modules_dir, custom_cfg_dir);

    initBrakesControl(modules_dir, custom_cfg_dir);

    initBrakesEquipment(modules_dir, custom_cfg_dir);

    initEPB(modules_dir, custom_cfg_dir);

    initElectroTransmission(modules_dir, custom_cfg_dir);

    initSafetyDevices(modules_dir, custom_cfg_dir);

    initOther(modules_dir, custom_cfg_dir);

    initMSUT(modules_dir, custom_cfg_dir);

    initAutostart();
/*
    reg = new Registrator(0.1);
    reg->setFileName("tep70-char");
    reg->init();
*/
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::preStep(double t)
{
    preStepCouplings(t);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::step(double t, double dt)
{
    stepCouplings(t, dt);

    stepCabineControls(t, dt);

    stepControlCircuit(t, dt);

    stepFuelSystem(t, dt);

    stepDisel(t, dt);

    stepOilSystem(t, dt);

    stepPneumoSupply(t, dt);

    stepBrakesControl(t, dt);

    stepBrakesEquipment(t, dt);

    stepEPB(t, dt);

    stepElectroTransmission(t, dt);

    stepSafetyDevices(t, dt);

    stepOther(t, dt);

    stepSignalsOutput(t, dt);

    stepMSUTsignals(t, dt);

    stepMSUT(t, dt);

    stepAutostart(t, dt);

    debugOutput(t, dt);

    if (reg == nullptr)
        return;

    QString line = QString("%1 %2 %3")
            .arg(velocity * Physics::kmh, 6, 'f', 1)
            .arg(tracForce / 1000.0, 6, 'f', 1)
            .arg(motor[0]->getAncorCurrent(), 6, 'f', 1);
    reg->print(line, t, dt);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::loadConfig(QString cfg_path)
{
    CfgReader cfg;

    if (cfg.load(cfg_path))
    {
        QString secName = "Vehicle";

        cfg.getString(secName, "CouplingModule", coupling_module_name);
        cfg.getString(secName, "CouplingConfig", coupling_config_name);
        cfg.getString(secName, "BrakeCraneModule", brake_crane_module_name);
        cfg.getString(secName, "BrakeCraneConfig", brake_crane_config_name);
        cfg.getString(secName, "LocoCraneModule", loco_crane_module_name);
        cfg.getString(secName, "LocoCraneConfig", loco_crane_config_name);
        cfg.getString(secName, "AirDistModule", airdist_module_name);
        cfg.getString(secName, "AirDistConfig", airdist_config_name);
        cfg.getString(secName, "ElectroAirDistModule", electro_airdist_module_name);
        cfg.getString(secName, "ElectroAirDistConfig", electro_airdist_config_name);

        double fuel_capacity = 0;
        cfg.getDouble(secName, "FuelCapacity", fuel_capacity);

        double fuel_level = 0;
        cfg.getDouble(secName, "FuelLevel", fuel_level);

        fuel_tank = new FuelTank();
        fuel_tank->setCapacity(fuel_capacity);
        fuel_tank->setFuelLevel(fuel_level);

        cfg.getDouble(secName, "ReductorCoeff", ip);
    }
}

GET_VEHICLE(TEP70BS)
