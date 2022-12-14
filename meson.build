project('obmc-yadro-clio', 'cpp',
  default_options: [
    'warning_level=3',
    'werror=true',
    'cpp_std=c++17',
  ],
  license: 'Apache-2.0',
  version: run_command(
    '/usr/bin/git',
    '--git-dir=@0@/.git'.format(meson.source_root()),
    'describe', '--always', '--dirty', '--long',
    check: true,
  ).stdout().strip(),
)

conf = configuration_data()
conf.set_quoted('PROJECT_VERSION', meson.project_version())
# Place additional config options here

configure_file(output: 'config.h', configuration: conf)

# CLI11 might not have a pkg-config. It is header only so just make
# sure we can access the needed symbols from the header.
cli11_dep = dependency('CLI11', required: false)
has_cli11 = meson.get_compiler('cpp').has_header_symbol(
  'CLI/CLI.hpp',
  'CLI::App',
  dependencies: cli11_dep,
  required: false,
)
if not has_cli11
  cli11_proj = subproject('cli11', required: false)
  assert(cli11_proj.found(), 'CLI11 is required')
  cli11_dep = cli11_proj.get_variable('CLI11_dep')
endif

executable('srvctl',
  [
    'src/srvctl/main.cpp',
  ],
  dependencies: [
    cli11_dep,
  ],
  install: true,
  # Hide the tool from unpreveleged users
  install_dir: get_option('sbindir'),
)
