cargs = ['-DG_LOG_DOMAIN="FuPluginTiTps6598x"']
plugins += {meson.current_source_dir().split('/')[-1]: true}

plugin_quirks += files('ti-tps6598x.quirk')
plugin_builtins += static_library('fu_plugin_ti_tps6598x',
  rustgen.process('fu-ti-tps6598x.rs'),
  sources: [
    'fu-ti-tps6598x-common.c',
    'fu-ti-tps6598x-device.c',
    'fu-ti-tps6598x-firmware.c',
    'fu-ti-tps6598x-pd-device.c',
    'fu-ti-tps6598x-plugin.c',
  ],
  include_directories: plugin_incdirs,
  link_with: plugin_libs,
  c_args: cargs,
  dependencies: plugin_deps,
)

device_tests += files(
  'tests/caldigit-ts4.json',
)
