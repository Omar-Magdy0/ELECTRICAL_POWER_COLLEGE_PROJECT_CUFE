#include "defaultSetting.h"
#include <string>
#include <sstream>
#include <json.hpp>




using json = nlohmann::ordered_json;
void default_settings_init(json *def){
  signal_defaults(def);
  electrical_defaults(def);
}

//BASIC SETTING FOR SIGNALS
void signal_defaults(json *def){
  (*def)["signal"]["export_path"] = R"(signals/)" ;
  (*def)["signal"]["import_path"] = R"(signals/)" ;
  (*def)["signal"]["min_max_accuracy"] = std::to_string(0.05);
  (*def)["signal"]["freq_diff"] = std::to_string(0.05);
  (*def)["signal"]["samplingRate_diff"] = std::to_string(0.05);
  (*def)["signal"]["maxima_diff_rounding"] = std::to_string(0.001);
  (*def)["signal"]["minima_diff_rounding"] = std::to_string(0.1);
  (*def)["signal"]["smaller_extremas_ignored"] = std::to_string(false);
  (*def)["signal"]["period_diff_accuracy"] = std::to_string(0.1);
  (*def)["signal"]["_COMMENT_FREQ_CALC_TYPE"] = "1 - trigger_level_based // "
                                                "2 - peak & trough_based";
  (*def)["signal"]["frequency_calc_type"] = std::to_string(1);
  (*def)["signal"]["periodic_avg&rms"] = std::to_string(false);
  (*def)["signal"]["interpolate_technique"] = "none";
}

void electrical_defaults(json *def){
  (*def)["electrical"]["assume_static_load"] = std::to_string(false);
  (*def)["electrical"]["use_rms_for_apparent"] = std::to_string(true);
  (*def)["electrical"]["___COMMENTS____"] = "FOR NON STATIC LOADING TRY RAISING THE FREQ_DIFF \n"
                                          " A BIT AND PERIOD DIFF ACCURACY TRY \n"
                                          "ALSO SWITCHING ON SMALL EXTREMAS IGNORED \n";
}
