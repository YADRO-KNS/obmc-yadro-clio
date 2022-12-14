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

fmt_dep = dependency('fmt')

executable('srvctl',
  [
    'src/srvctl/main.cpp',
  ],
  dependencies: [
    fmt_dep,
  ],
  install: true,
  # Hide the tool from unpreveleged users
  install_dir: get_option('sbindir'),
)
