#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::preStepHoses(const double& t)
{
    Q_UNUSED(t)

    hose_fl_fwd->setVehiclePosition(profile_point_data.position,
                                    profile_point_data.right,
                                    profile_point_data.orth,
                                    profile_point_data.up);

    hose_fl_bwd->setVehiclePosition(profile_point_data.position,
                                    -profile_point_data.right,
                                    -profile_point_data.orth,
                                    profile_point_data.up);

    hose_bp_fwd->setVehiclePosition(profile_point_data.position,
                                    profile_point_data.right,
                                    profile_point_data.orth,
                                    profile_point_data.up);

    hose_bp_bwd->setVehiclePosition(profile_point_data.position,
                                    -profile_point_data.right,
                                    -profile_point_data.orth,
                                    profile_point_data.up);

    hose_bc_fwd->setVehiclePosition(profile_point_data.position,
                                    profile_point_data.right,
                                    profile_point_data.orth,
                                    profile_point_data.up);

    hose_bc_bwd->setVehiclePosition(profile_point_data.position,
                                    -profile_point_data.right,
                                    -profile_point_data.orth,
                                    profile_point_data.up);
}