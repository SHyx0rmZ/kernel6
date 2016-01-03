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

task :default => [ :directories, :tools, :templates, :compilation ]  do |t|
    puts '> done'
end

task :directories do
    verbose(false) do
        DIRECTORIES.values.each do |path|
            Rake::Task[path].invoke
        end
    end
end

file 'build/config.yml' => [ 'config.yml', 'generate' ] do |f|
    puts "> ruby config.yml"

    verbose(false) do
        mkdir_p(File.dirname(f.name))
        ruby "generate #{f.source} #{f.name}"
    end
end

file 'config.yml'
file 'generate'
file 'instance'
file 'shared'

task :tools => [ 'build/config.yml', 'instance', 'shared' ]

task :templates => TEMPLATES do |t|
end

rule /^build\/src\/shared\/.*\.hpp$/ => ->(t){ t.pathmap('%{^build/src/,src/}p') } do |t|
    puts "> ruby #{t.name}"

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

task :compilation => 'build/out/kernel6'

file 'build/out/kernel6' => 'build/out/loader' do |f|
    puts "> cp   kernel6"

    verbose(false) do
        cp(f.source, f.name)
    end
end

file 'build/out/loader' => [ 'build/obj/payload', 'build/obj/_udivdi3.o', 'build/obj/_umoddi3.o', *OBJECTS_LOADER, *OBJECTS_SHARED_X86, 'build/out/libnukexx.x86.a', 'build/src/script_loader.ld' ] do |f|
    puts "> ld   loader"

    verbose(false) do
        sh %| ld #{LDFLAGS} -o #{f.name} #{f.sources.join(' ')} -T build/src/script_loader.ld |
    end
end

file 'build/obj/payload' => [ 'build/out/kernel', 'build/src/payload.S' ] do |f|
    puts "> g++  #{f.name.pathmap('%{^build/obj/,}p}')}"

    verbose(false) do
        sh %| g++ #{CXXFLAGS} -m32 -c build/src/payload.S -o #{f.name} |
    end
end

file 'build/obj/_udivdi3.o' do |f|
    puts "> ar   #{f.name.pathmap('%{^build/obj/,}p')}"

    verbose(false) do
        sh %| ar -x #{%x| g++ -m32 --print-libgcc-file-name |.strip} _udivdi3.o |
        mv('_udivdi3.o', 'build/obj/')
    end
end

file 'build/obj/_umoddi3.o' do |f|
    puts "> ar   #{f.name.pathmap('%{,build/obj/}p')}"

    verbose(false) do
        sh %| ar -x #{%x| g++ -m32 --print-libgcc-file-name |.strip} _umoddi3.o |
        mv('_umoddi3.o', 'build/obj/')
    end
end

rule /^build\/obj\/loader\/.*\.cpp\.o$/ => ->(t){ t.pathmap('%{^build/obj/,build/src/}p').ext('') } do |t|
    puts "> g++  #{t.source.pathmap('%{^build/src/,}p')}"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        sh %| g++ #{CXXFLAGS} -Ibuild/src/loader -Ibuild/src/shared/x86 -isystem build/src/lib -m32 -fno-exceptions -c #{t.source} -o #{t.name} |
    end
end

rule /^build\/obj\/loader\/.*\.S\.o$/ => ->(t){ t.pathmap('%{^build/obj/,build/src/}p').ext('') } do |t|
    puts "> g++  #{t.source.pathmap('%{^build/src/,}p')}"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        sh %| g++ #{CXXFLAGS} -Ibuild/src/loader -Ibuild/src/shared/x86 -isystem build/src/lib -m32 -fno-exceptions -c #{t.source} -o #{t.name} |
    end
end

# compilation, libnukexx

# rule /^build\/obj\/lib\/.*\.x86\.cpp\.o$/ => ->(t){ t.pathmap('%{^build/obj/,build/src/}p').ext('') } do |t|
#     puts "> g++  #{t.source.pathmap('%{^build/src/,}p')}"

#     verbose(false) do
#         mkdir_p(t.name.pathmap('%d'))
#         sh %| g++ #{CXXFLAGS} -Ibuild/src/shared/x86 -Ibuild/src/lib -isystem build/src/lib -m32 -fno-exceptions -c #{t.source} -o #{t.name} |
#     end
# end

# compilation, shared

rule /^build\/obj\/(?:lib|shared)\/.*\.x86\.(?:cpp|S)\.o$/ => ->(t){ t.pathmap('%{^build/obj/(.*)\.x86,build/src/\1}p').ext('') } do |t|
    puts "> g++  #{t.source.pathmap('%{^build/src/,}p')}"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        sh %| g++ #{CXXFLAGS} -Ibuild/src/shared/x86 -Ibuild/src/lib -isystem build/src/lib -m32 -fno-exceptions -c #{t.source} -o #{t.name} |
    end
end

rule /^build\/obj\/(?:lib|shared)\/.*\.x64\.(?:cpp|S)\.o$/ => ->(t){ t.pathmap('%{^build/obj/(.*)\.x64,build/src/\1}p').ext('') } do |t|
    puts "> g++  #{t.source.pathmap('%{^build/src/,}p')}"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        sh %| g++ #{CXXFLAGS} -Ibuild/src/shared/x64 -Ibuild/src/lib -isystem build/src/lib -m64 -c #{t.source} -o #{t.name} |
    end
end

file 'build/out/libnukexx.x86.a' => OBJECTS_LIBNUKEXX_X86 do |t|
    puts "> ar   libnukexx.x86.a"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        sh %| ar -rc #{t.name} #{t.sources.join(' ')} |
    end
end

file 'build/out/libnukexx.x64.a' => OBJECTS_LIBNUKEXX_X64 do |t|
    puts "> ar   libnukexx.x64.a"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        sh %| ar -rc #{t.name} #{t.sources.join(' ')} |
    end
end

file 'build/out/kernel' => [ *OBJECTS_KERNEL, *OBJECTS_SHARED_X64, 'build/out/libnukexx.x64.a', 'build/src/script_kernel.ld' ] do |t|
    puts "> ld   kernel"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        sh %| ld #{LDFLAGS} -o #{t.name} #{t.sources.join(' ')} -T build/src/script_kernel.ld |
    end
end

rule /^build\/obj\/kernel\/.*\.(?:cpp|S)\.o$/ => ->(t){ t.pathmap('%{^build/obj/,build/src/}p').ext('') } do |t|
    puts "> g++  #{t.source.pathmap('%{^build/src/,}p')}"

    verbose(false) do
        mkdir_p(t.name.pathmap('%d'))
        sh %| g++ #{CXXFLAGS} -Ibuild/src/kernel -Ibuild/src/shared/x64 -isystem build/src/lib -m64 -c #{t.source} -o #{t.name} |
    end
end

task :clean do
    verbose(false) do
        rm_r('build') if Dir.exists?('build')
    end
end

task :qemu => :default do
    verbose(false) do
        begin
            sh %| qemu-system-x86_64 -kernel build/out/kernel6 -smp 2 -m 128M -serial stdio |
        rescue SystemExit, Interrupt
            sh %| reset |
            raise
        end
    end
end