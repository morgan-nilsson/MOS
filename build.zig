const std = @import("std");
const Builder = std.build.Builder;
const Target = std.build.Target;

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{ .default_target = .{
        .cpu_arch = .x86,
        .os_tag = .freestanding,
    } });
    const optimize = b.standardOptimizeOption(.{
        .preferred_optimize_mode = .ReleaseSmall,
    });

    //const stdio = b.createModule(.{
    //    .root_source_file = b.path("src/libs/stdio.zig"),
    //});

    //const vga_driver = b.createModule(.{ .root_source_file = b.path("src/driver/vga_driver.zig"), .imports = &.{.{ .name = "stdio", .module = stdio, }} });

    const kernel = b.addExecutable(.{
        .name = "kernel.out",
        .root_source_file = b.path("src/kernel/kernel.zig"),
        .target = target,
        .optimize = optimize,
        .single_threaded = true,
        .strip = true,
        .linkage = .static,
        .error_tracing = false,
        .unwind_tables = .none,
    });

    kernel.addObjectFile(b.path("zig-out/bin/kernel/kernel-entry.o"));
    //kernel.addObjectFile(b.path("zig-out/bin/kernel/interrupt.o"));

    //kernel.root_module.addImport("vga_driver", vga_driver);

    kernel.addCSourceFile(.{
        .file = b.path("src/kernel/_start.c"),
    });

    kernel.setLinkerScript(b.path("linker.ld"));

    b.installArtifact(kernel);
}
