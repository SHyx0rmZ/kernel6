#!/usr/bin/env ruby

require 'rake'

module ConstRedef
    def const_redef(const, value)
        self.class.send(:remove_const, const) if self.class.const_defined?(const)
        self.class.const_set(const, value)
    end
end

class << self
    include ConstRedef
end

CXXFLAGS = '-g -g3 -Wall -Wextra -Werror -std=gnu++11 -ffreestanding -nostdlib -nostartfiles -nostdinc++ -fno-leading-underscore -O0 -fno-rtti -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow'
LDFLAGS = '-n'

DIRECTORIES = { :KERNEL => 'kernel', :LOADER => 'loader', :LIBNUKEXX => 'lib', :SHARED => 'shared' }

DIRECTORIES.each_pair do |key, value|
    self.class.const_set("SOURCES_#{key}", FileList[ "src/#{value}/**/*.cpp", "src/#{value}/**/*.S" ])
    self.class.const_set("HEADERS_#{key}", FileList[ "src/#{value}/**/*.hpp" ])
end

const_redef :HEADERS_LIBNUKEXX, FileList[ 'src/lib/**/*' ] - SOURCES_LIBNUKEXX

OBJECTS_KERNEL = SOURCES_KERNEL.pathmap('%{^src/,build/obj/}p.o')
OBJECTS_LOADER = SOURCES_LOADER.pathmap('%{^src/,build/obj/}p.o') - [ 'build/obj/loader/smpinit.S.o' ]
OBJECTS_LIBNUKEXX_X86 = SOURCES_LIBNUKEXX.pathmap('%{^src/,build/obj/}X.x86%x.o')
OBJECTS_LIBNUKEXX_X64 = SOURCES_LIBNUKEXX.pathmap('%{^src/,build/obj/}X.x64%x.o')
OBJECTS_SHARED_X86 = SOURCES_SHARED.pathmap('%{^src/,build/obj/}X.x86%x.o')
OBJECTS_SHARED_X64 = SOURCES_SHARED.pathmap('%{^src/,build/obj/}X.x64%x.o')

SCRIPTS = FileList[ 'src/**/*.ld' ]

%i[ SOURCES HEADERS OBJECTS ].each do |type|
    self.class.const_set(type, FileList[ self.class.constants.select{ |c| c[/^#{type}_/] }.map{ |c| self.class.const_get(c) }.flatten ])
end

TEMPLATES = FileList[ [ HEADERS, SOURCES, SCRIPTS ].map { |list| list.pathmap('%{^src/,build/src/}p') }.flatten ]

DIRECTORIES.each_value do |path|
    directory(path)
end

task :default => [ :directories, :tools, :templates]  do |t|
    puts '> done'
end

task :directories do
    verbose(false) do
        DIRECTORIES.values.each do |path|
            Rake::Task[path].invoke
        end
    end
end

file 'build/config.yml'
file 'instance'
file 'shared'

task :tools => [ 'build/config.yml', 'instance', 'shared' ]

task :templates => TEMPLATES do |t|
end

rule /^build\/src\/shared\/.*\.hpp$/ => ->(t){ t.pathmap('%{^build/src/,src/}p') } do |t|
    puts "> ruby s #{t.name}"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        mkdir_p(File.dirname(t.name.pathmap('%{^build/src/shared/,build/src/shared/x64/}p')))
        mkdir_p(File.dirname(t.name.pathmap('%{^build/src/shared/,build/src/shared/x86/}p')))
        ruby "instance build/config.yml #{t.source} #{t.name}"
        ruby "shared amd64 #{t.name} " + t.name.pathmap('%{^build/src/shared/,build/src/shared/x64/}p')
        ruby "shared i386 #{t.name} " + t.name.pathmap('%{^build/src/shared/,build/src/shared/x86/}p')
    end
end

rule /^build\/src\/.*$/ => ->(t){ t.pathmap('%{^build/src/,src/}p') } do |t|
    puts "> ruby #{t.name}"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        ruby "instance build/config.yml #{t.source} #{t.name}"
    end
end


