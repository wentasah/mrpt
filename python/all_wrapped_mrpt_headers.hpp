// Include file for the "binder" tool.
// https://cppbinder.readthedocs.io/

// core
#include <mrpt/core/Clock.h>
#include <mrpt/core/Stringifyable.h>
#include <mrpt/core/WorkerThreadsPool.h>
#include <mrpt/core/abs_diff.h>
#include <mrpt/core/backtrace.h>
#include <mrpt/core/bits_math.h>
#include <mrpt/core/common.h>
#include <mrpt/core/cpu.h>
#include <mrpt/core/from_string.h>
#include <mrpt/core/optional_ref.h>
#include <mrpt/core/reverse_bytes.h>
#include <mrpt/core/round.h>

// random
#include <mrpt/random/RandomGenerators.h>

// io
#include <mrpt/io/CFileGZInputStream.h>
#include <mrpt/io/CFileGZOutputStream.h>
#include <mrpt/io/CFileInputStream.h>
#include <mrpt/io/CFileOutputStream.h>
#include <mrpt/io/CFileStream.h>
#include <mrpt/io/CMemoryStream.h>
#include <mrpt/io/CStream.h>
#include <mrpt/io/CTextFileLinesParser.h>
#include <mrpt/io/lazy_load_path.h>
#include <mrpt/io/open_flags.h>
#include <mrpt/io/vector_loadsave.h>
#include <mrpt/io/zip.h>

// serialization
#include <mrpt/serialization/CArchive.h>
#include <mrpt/serialization/CSerializable.h>
#include "./serialization-python-instances.h"

// rtti
#include <mrpt/rtti/CObject.h>

// bayes
#include <mrpt/bayes/CParticleFilter.h>
#include <mrpt/bayes/CParticleFilterCapable.h>

// apps
#include <mrpt/apps/ICP_SLAM_App.h>
#include <mrpt/apps/KFSLAMApp.h>
#include <mrpt/apps/MonteCarloLocalization_App.h>
#include <mrpt/apps/RBPF_SLAM_App.h>
#include <mrpt/apps/RawlogEditApp.h>
#include <mrpt/apps/RawlogGrabberApp.h>

// comms
#include <mrpt/comms/CClientTCPSocket.h>
#include <mrpt/comms/CSerialPort.h>
#include <mrpt/comms/CServerTCPSocket.h>
#include <mrpt/comms/net_utils.h>

// config
#include <mrpt/config/CConfigFile.h>
#include <mrpt/config/CConfigFileBase.h>
#include <mrpt/config/CConfigFileMemory.h>
#include <mrpt/config/CConfigFilePrefixer.h>
#include <mrpt/config/CLoadableOptions.h>
#include <mrpt/config/config_parser.h>

// expr
#include <mrpt/expr/CRuntimeCompiledExpression.h>

// gui
#include <mrpt/gui.h>

// hwdrivers
#include <mrpt/hwdrivers/C2DRangeFinderAbstract.h>
#include <mrpt/hwdrivers/CBoardENoses.h>
#include <mrpt/hwdrivers/CBoardSonars.h>
#include <mrpt/hwdrivers/CCANBusReader.h>
#include <mrpt/hwdrivers/CCameraSensor.h>
#include <mrpt/hwdrivers/CDUO3DCamera.h>
#include <mrpt/hwdrivers/CEnoseModular.h>
#include <mrpt/hwdrivers/CFFMPEG_InputStream.h>
#include <mrpt/hwdrivers/CGPSInterface.h>
#include <mrpt/hwdrivers/CGPS_NTRIP.h>
#include <mrpt/hwdrivers/CGenericSensor.h>
#include <mrpt/hwdrivers/CGillAnemometer.h>
#include <mrpt/hwdrivers/CGyroKVHDSP3000.h>
#include <mrpt/hwdrivers/CHokuyoURG.h>
#include <mrpt/hwdrivers/CIMUIntersense.h>
#include <mrpt/hwdrivers/CIMUXSens_MT4.h>
#include <mrpt/hwdrivers/CIbeoLuxETH.h>
#include <mrpt/hwdrivers/CImageGrabber_FlyCapture2.h>
#include <mrpt/hwdrivers/CImageGrabber_OpenCV.h>
#include <mrpt/hwdrivers/CImageGrabber_dc1394.h>
#include <mrpt/hwdrivers/CImpinjRFID.h>
#include <mrpt/hwdrivers/CJoystick.h>
#include <mrpt/hwdrivers/CKinect.h>
#include <mrpt/hwdrivers/CLMS100eth.h>
#include <mrpt/hwdrivers/CMyntEyeCamera.h>
#include <mrpt/hwdrivers/CNTRIPClient.h>
#include <mrpt/hwdrivers/CNTRIPEmitter.h>
#include <mrpt/hwdrivers/CNationalInstrumentsDAQ.h>
#include <mrpt/hwdrivers/COpenNI2Generic.h>
#include <mrpt/hwdrivers/COpenNI2Sensor.h>
#include <mrpt/hwdrivers/COpenNI2_RGBD360.h>
#include <mrpt/hwdrivers/CPhidgetInterfaceKitProximitySensors.h>
#include <mrpt/hwdrivers/CPtuBase.h>
#include <mrpt/hwdrivers/CPtuDPerception.h>
#include <mrpt/hwdrivers/CRaePID.h>
#include <mrpt/hwdrivers/CRoboPeakLidar.h>
#include <mrpt/hwdrivers/CRoboticHeadInterface.h>
#include <mrpt/hwdrivers/CSICKTim561Eth_2050101.h>
#include <mrpt/hwdrivers/CServoeNeck.h>
#include <mrpt/hwdrivers/CSickLaserSerial.h>
#include <mrpt/hwdrivers/CSickLaserUSB.h>
#include <mrpt/hwdrivers/CSkeletonTracker.h>
#include <mrpt/hwdrivers/CStereoGrabber_Bumblebee_libdc1394.h>
#include <mrpt/hwdrivers/CStereoGrabber_SVS.h>
#include <mrpt/hwdrivers/CSwissRanger3DCamera.h>
#include <mrpt/hwdrivers/CTaoboticsIMU.h>
#include <mrpt/hwdrivers/CTuMicos.h>
#include <mrpt/hwdrivers/CVelodyneScanner.h>
#include <mrpt/hwdrivers/CWirelessPower.h>

// img
#include <mrpt/img/CImage.h>
#include <mrpt/img/DistortionModel.h>
#include <mrpt/img/TCamera.h>
#include <mrpt/img/TColor.h>
#include <mrpt/img/TPixelCoord.h>
#include <mrpt/img/TStereoCamera.h>
#include <mrpt/img/color_maps.h>

// kinematics
#include <mrpt/kinematics/CKinematicChain.h>
#include <mrpt/kinematics/CVehicleSimulVirtualBase.h>
#include <mrpt/kinematics/CVehicleSimul_DiffDriven.h>
#include <mrpt/kinematics/CVehicleSimul_Holo.h>
#include <mrpt/kinematics/CVehicleVelCmd.h>
#include <mrpt/kinematics/CVehicleVelCmd_DiffDriven.h>
#include <mrpt/kinematics/CVehicleVelCmd_Holo.h>

// maps:
#include <mrpt/maps/CBeacon.h>
#include <mrpt/maps/CBeaconMap.h>
#include <mrpt/maps/CColouredOctoMap.h>
#include <mrpt/maps/CColouredPointsMap.h>
#include <mrpt/maps/CGasConcentrationGridMap2D.h>
#include <mrpt/maps/CHeightGridMap2D.h>
#include <mrpt/maps/CHeightGridMap2D_MRF.h>
#include <mrpt/maps/CLogOddsGridMap2D.h>
#include <mrpt/maps/CLogOddsGridMap3D.h>
#include <mrpt/maps/CLogOddsGridMapLUT.h>
#include <mrpt/maps/CMultiMetricMap.h>
#include <mrpt/maps/COccupancyGridMap2D.h>
#include <mrpt/maps/COccupancyGridMap3D.h>
#include <mrpt/maps/COctoMap.h>
#include <mrpt/maps/CPointCloudFilterByDistance.h>
#include <mrpt/maps/CPointsMap.h>
#include <mrpt/maps/CPointsMapXYZI.h>
#include <mrpt/maps/CRandomFieldGridMap2D.h>
#include <mrpt/maps/CRandomFieldGridMap3D.h>
#include <mrpt/maps/CReflectivityGridMap2D.h>
#include <mrpt/maps/CSimplePointsMap.h>
#include <mrpt/maps/CWeightedPointsMap.h>
#include <mrpt/maps/CWirelessPowerGridMap2D.h>
#include <mrpt/maps/OccupancyGridCellType.h>
#include <mrpt/obs/CObservationPointCloud.h>
#include <mrpt/obs/CObservationRotatingScan.h>
#include <mrpt/obs/customizable_obs_viz.h>
#include <mrpt/opengl/CAngularObservationMesh.h>
#include <mrpt/opengl/CPlanarLaserScan.h>

// ma.h>

#include <mrpt/math/CAtan2LookUpTable.h>
#include <mrpt/math/CHistogram.h>
#include <mrpt/math/CLevenbergMarquardt.h>
#include <mrpt/math/CMatrixB.h>
#include <mrpt/math/CMatrixD.h>
#include <mrpt/math/CMatrixDynamic.h>
#include <mrpt/math/CMatrixF.h>
#include <mrpt/math/CMatrixFixed.h>
//#include <mrpt/math/CMonteCarlo.h>
#include <mrpt/math/CPolygon.h>
#include <mrpt/math/CProbabilityDensityFunction.h>
#include <mrpt/math/CQuaternion.h>
//#include <mrpt/math/CSparseMatrix.h>
#include <mrpt/math/CSparseMatrixTemplate.h>
#include <mrpt/math/CSplineInterpolator1D.h>
#include <mrpt/math/CVectorDynamic.h>
#include <mrpt/math/CVectorFixed.h>
#include <mrpt/math/KDTreeCapable.h>
//#include <mrpt/math/MatrixBase.h>
//#include <mrpt/math/MatrixBlockSparseCols.h>
//#include <mrpt/math/MatrixVectorBase.h>
#include <mrpt/math/TBoundingBox.h>
#include <mrpt/math/TLine2D.h>
#include <mrpt/math/TLine3D.h>
#include <mrpt/math/TObject2D.h>
#include <mrpt/math/TObject3D.h>
#include <mrpt/math/TPlane.h>
#include <mrpt/math/TPoint2D.h>
#include <mrpt/math/TPoint3D.h>
#include <mrpt/math/TPolygon2D.h>
#include <mrpt/math/TPolygon3D.h>
#include <mrpt/math/TPolygonWithPlane.h>
#include <mrpt/math/TPose2D.h>
#include <mrpt/math/TPose3D.h>
#include <mrpt/math/TPose3DQuat.h>
#include <mrpt/math/TPoseOrPoint.h>
#include <mrpt/math/TSegment2D.h>
#include <mrpt/math/TSegment3D.h>
#include <mrpt/math/TTwist2D.h>
#include <mrpt/math/TTwist3D.h>
#include <mrpt/math/data_utils.h>
#include <mrpt/math/distributions.h>
//#include <mrpt/math/eigen_extensions.h>
#include <mrpt/math/epsilon.h>
#include <mrpt/math/filters.h>
#include <mrpt/math/fourier.h>
#include <mrpt/math/fresnel.h>
#include <mrpt/math/geometry.h>
//#include <mrpt/math/gtsam_wrappers.h>
#include <mrpt/math/homog_matrices.h>
#include <mrpt/math/interp_fit.h>
#include <mrpt/math/kmeans.h>
//#include <mrpt/math/lightweight_geom_data.h>
//#include <mrpt/math/lightweight_geom_data_frwds.h>
//#include <mrpt/math/mat2eig.h>
//#include <mrpt/math/math_frwds.h>
//#include <mrpt/math/matrix_adaptors.h>
//#include <mrpt/math/matrix_serialization.h>
#include <mrpt/math/matrix_size_t.h>
#include <mrpt/math/model_search.h>
#include <mrpt/math/model_search_impl.h>
#include <mrpt/math/num_jacobian.h>
#include <mrpt/math/ops_containers.h>
#include <mrpt/math/ops_matrices.h>
#include <mrpt/math/ops_vectors.h>
#include <mrpt/math/point_poses2vectors.h>
#include <mrpt/math/poly_roots.h>
#include <mrpt/math/ransac.h>
#include <mrpt/math/ransac_applications.h>
#include <mrpt/math/ransac_impl.h>
#include <mrpt/math/robust_kernels.h>
#include <mrpt/math/slerp.h>
#include <mrpt/math/transform_gaussian.h>
#include <mrpt/math/utils.h>
//#include <mrpt/math/utils_matlab.h>
#include <mrpt/math/wrap2pi.h>

// nav
#include <mrpt/nav/holonomic/CAbstractHolonomicReactiveMethod.h>
#include <mrpt/nav/holonomic/CHolonomicFullEval.h>
#include <mrpt/nav/holonomic/CHolonomicLogFileRecord.h>
#include <mrpt/nav/holonomic/CHolonomicND.h>
#include <mrpt/nav/holonomic/CHolonomicVFF.h>
#include <mrpt/nav/holonomic/ClearanceDiagram.h>
#include <mrpt/nav/planners/PlannerRRT_SE2_TPS.h>
#include <mrpt/nav/planners/PlannerRRT_common.h>
#include <mrpt/nav/planners/PlannerSimple2D.h>
#include <mrpt/nav/planners/TMoveTree.h>
#include <mrpt/nav/planners/impl_renderMoveTree.h>
#include <mrpt/nav/planners/nav_plan_geometry_utils.h>
#include <mrpt/nav/reactive/CAbstractNavigator.h>
#include <mrpt/nav/reactive/CAbstractPTGBasedReactive.h>
#include <mrpt/nav/reactive/CLogFileRecord.h>
#include <mrpt/nav/reactive/CMultiObjMotionOpt_Scalarization.h>
#include <mrpt/nav/reactive/CMultiObjectiveMotionOptimizerBase.h>
#include <mrpt/nav/reactive/CNavigatorManualSequence.h>
#include <mrpt/nav/reactive/CReactiveNavigationSystem.h>
#include <mrpt/nav/reactive/CReactiveNavigationSystem3D.h>
#include <mrpt/nav/reactive/CRobot2NavInterface.h>
#include <mrpt/nav/reactive/CRobot2NavInterfaceForSimulator.h>
#include <mrpt/nav/reactive/CWaypointsNavigator.h>
#include <mrpt/nav/reactive/TCandidateMovementPTG.h>
#include <mrpt/nav/reactive/TWaypoint.h>
#include <mrpt/nav/tpspace/CPTG_DiffDrive_C.h>
#include <mrpt/nav/tpspace/CPTG_DiffDrive_CC.h>
#include <mrpt/nav/tpspace/CPTG_DiffDrive_CCS.h>
#include <mrpt/nav/tpspace/CPTG_DiffDrive_CS.h>
#include <mrpt/nav/tpspace/CPTG_DiffDrive_CollisionGridBased.h>
#include <mrpt/nav/tpspace/CPTG_DiffDrive_alpha.h>
#include <mrpt/nav/tpspace/CPTG_Holo_Blend.h>
#include <mrpt/nav/tpspace/CParameterizedTrajectoryGenerator.h>

// obs
#include <mrpt/maps/CMetricMap.h>
#include <mrpt/maps/CMetricMapEvents.h>
#include <mrpt/maps/CSimpleMap.h>
#include <mrpt/maps/TMetricMapInitializer.h>
#include <mrpt/maps/TMetricMapTypesRegistry.h>
#include <mrpt/maps/metric_map_types.h>
#include <mrpt/obs/CAction.h>
#include <mrpt/obs/CActionCollection.h>
#include <mrpt/obs/CActionRobotMovement2D.h>
#include <mrpt/obs/CActionRobotMovement3D.h>
#include <mrpt/obs/CObservation.h>
#include <mrpt/obs/CObservation2DRangeScan.h>
#include <mrpt/obs/CObservation2DRangeScanWithUncertainty.h>
#include <mrpt/obs/CObservation3DRangeScan.h>
#include <mrpt/obs/CObservation3DRangeScan_project3D_impl.h>
#include <mrpt/obs/CObservation3DScene.h>
#include <mrpt/obs/CObservation6DFeatures.h>
#include <mrpt/obs/CObservationBatteryState.h>
#include <mrpt/obs/CObservationBeaconRanges.h>
#include <mrpt/obs/CObservationBearingRange.h>
#include <mrpt/obs/CObservationCANBusJ1939.h>
#include <mrpt/obs/CObservationComment.h>
#include <mrpt/obs/CObservationGPS.h>
#include <mrpt/obs/CObservationGasSensors.h>
#include <mrpt/obs/CObservationIMU.h>
#include <mrpt/obs/CObservationImage.h>
#include <mrpt/obs/CObservationOdometry.h>
#include <mrpt/obs/CObservationRFID.h>
#include <mrpt/obs/CObservationRGBD360.h>
#include <mrpt/obs/CObservationRange.h>
#include <mrpt/obs/CObservationRawDAQ.h>
#include <mrpt/obs/CObservationReflectivity.h>
#include <mrpt/obs/CObservationRobotPose.h>
#include <mrpt/obs/CObservationSkeleton.h>
#include <mrpt/obs/CObservationStereoImages.h>
#include <mrpt/obs/CObservationStereoImagesFeatures.h>
#include <mrpt/obs/CObservationVelodyneScan.h>
#include <mrpt/obs/CObservationWindSensor.h>
#include <mrpt/obs/CObservationWirelessPower.h>
#include <mrpt/obs/CRawlog.h>
#include <mrpt/obs/CSensoryFrame.h>
#include <mrpt/obs/CSinCosLookUpTableFor2DScans.h>
#include <mrpt/obs/T2DScanProperties.h>
#include <mrpt/obs/T3DPointsProjectionParams.h>
#include <mrpt/obs/T3DPointsTo2DScanParams.h>
#include <mrpt/obs/TPixelLabelInfo.h>
#include <mrpt/obs/TRangeImageFilter.h>
#include <mrpt/obs/VelodyneCalibration.h>
#include <mrpt/obs/carmen_log_tools.h>
#include <mrpt/obs/format_externals_filename.h>
#include <mrpt/obs/gnss_messages.h>
#include <mrpt/obs/gnss_messages_ascii_nmea.h>
#include <mrpt/obs/gnss_messages_common.h>
#include <mrpt/obs/gnss_messages_novatel.h>
#include <mrpt/obs/gnss_messages_topcon.h>
#include <mrpt/obs/gnss_messages_type_list.h>
#include <mrpt/obs/obs_utils.h>
#include <mrpt/obs/stock_observations.h>

// opengl
#include <mrpt/opengl.h>

// poses

#include <mrpt/poses/CPoint.h>
#include <mrpt/poses/CPoint2D.h>
#include <mrpt/poses/CPoint2DPDF.h>
#include <mrpt/poses/CPoint2DPDFGaussian.h>
#include <mrpt/poses/CPoint3D.h>
#include <mrpt/poses/CPointPDF.h>
#include <mrpt/poses/CPointPDFGaussian.h>
#include <mrpt/poses/CPointPDFParticles.h>
#include <mrpt/poses/CPointPDFSOG.h>
#include <mrpt/poses/CPose.h>
#include <mrpt/poses/CPose2D.h>
#include <mrpt/poses/CPose2DGridTemplate.h>
#include <mrpt/poses/CPose2DInterpolator.h>
#include <mrpt/poses/CPose3D.h>
#include <mrpt/poses/CPose3DGridTemplate.h>
#include <mrpt/poses/CPose3DInterpolator.h>
#include <mrpt/poses/CPose3DPDF.h>
#include <mrpt/poses/CPose3DPDFGaussian.h>
#include <mrpt/poses/CPose3DPDFGaussianInf.h>
#include <mrpt/poses/CPose3DPDFGrid.h>
#include <mrpt/poses/CPose3DPDFParticles.h>
#include <mrpt/poses/CPose3DPDFSOG.h>
#include <mrpt/poses/CPose3DQuat.h>
#include <mrpt/poses/CPose3DQuatPDF.h>
#include <mrpt/poses/CPose3DQuatPDFGaussian.h>
#include <mrpt/poses/CPose3DQuatPDFGaussianInf.h>
#include <mrpt/poses/CPoseInterpolatorBase.h>
#include <mrpt/poses/CPoseOrPoint.h>
#include <mrpt/poses/CPoseOrPoint_detail.h>
#include <mrpt/poses/CPosePDF.h>
#include <mrpt/poses/CPosePDFGaussian.h>
#include <mrpt/poses/CPosePDFGaussianInf.h>
#include <mrpt/poses/CPosePDFGrid.h>
#include <mrpt/poses/CPosePDFParticles.h>
#include <mrpt/poses/CPosePDFSOG.h>
#include <mrpt/poses/CPoseRandomSampler.h>
#include <mrpt/poses/CPoses2DSequence.h>
#include <mrpt/poses/CPoses3DSequence.h>
#include <mrpt/poses/CRobot2DPoseEstimator.h>
#include <mrpt/poses/FrameTransformer.h>
#include <mrpt/poses/Lie/Euclidean.h>
#include <mrpt/poses/Lie/SE.h>
#include <mrpt/poses/Lie/SO.h>
#include <mrpt/poses/SO_SE_average.h>
#include <mrpt/poses/sensor_poses.h>

// slam
#include <mrpt/maps/CMultiMetricMapPDF.h>
#include <mrpt/slam/CGridMapAligner.h>
#include <mrpt/slam/CICP.h>
#include <mrpt/slam/CIncrementalMapPartitioner.h>
#include <mrpt/slam/CMetricMapBuilder.h>
#include <mrpt/slam/CMetricMapBuilderICP.h>
#include <mrpt/slam/CMetricMapBuilderRBPF.h>
#include <mrpt/slam/CMetricMapsAlignmentAlgorithm.h>
#include <mrpt/slam/CMonteCarloLocalization2D.h>
#include <mrpt/slam/CMonteCarloLocalization3D.h>
#include <mrpt/slam/COccupancyGridMapFeatureExtractor.h>
#include <mrpt/slam/CRangeBearingKFSLAM.h>
#include <mrpt/slam/CRangeBearingKFSLAM2D.h>
#include <mrpt/slam/CRejectionSamplingRangeOnlyLocalization.h>
#include <mrpt/slam/PF_implementations.h>
#include <mrpt/slam/PF_implementations_data.h>
#include <mrpt/slam/TKLDParams.h>
#include <mrpt/slam/TMonteCarloLocalizationParams.h>
#include <mrpt/slam/data_association.h>
#include <mrpt/slam/observations_overlap.h>

// system
#include <mrpt/system/CConsoleRedirector.h>
#include <mrpt/system/CControlledRateTimer.h>
#include <mrpt/system/CDirectoryExplorer.h>
#include <mrpt/system/CFileSystemWatcher.h>
#include <mrpt/system/CObservable.h>
#include <mrpt/system/CObserver.h>
// #include <mrpt/system/COutputLogger.h> // Failed
#include <mrpt/system/CRateTimer.h>
#include <mrpt/system/CTicTac.h>
#include <mrpt/system/CTimeLogger.h>
#include <mrpt/system/WorkerThreadsPool.h>
#include <mrpt/system/crc.h>
#include <mrpt/system/datetime.h>
#include <mrpt/system/filesystem.h>
#include <mrpt/system/md5.h>
#include <mrpt/system/memory.h>
#include <mrpt/system/mrptEvent.h>
//#include <mrpt/system/os.h>  // Failed
#include <mrpt/system/progress.h>
#include <mrpt/system/scheduler.h>
#include <mrpt/system/string_utils.h>
#include <mrpt/system/thread_name.h>

// tfest
#include <mrpt/tfest/TMatchingPair.h>
#include <mrpt/tfest/indiv-compat-decls.h>
#include <mrpt/tfest/se2.h>
#include <mrpt/tfest/se3.h>

// topography
#include <mrpt/topography/conversions.h>
#include <mrpt/topography/data_types.h>
#include <mrpt/topography/path_from_rtk_gps.h>
