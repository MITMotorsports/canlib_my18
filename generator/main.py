import sys
import os
import jinja2
from jinja2 import nodes
from jinja2.ext import Extension
from jinja2.exceptions import TemplateRuntimeError
from pathlib import Path

import ParseCAN.ParseCAN as ParseCAN

import constants
import computers_hpp
import computers_cpp
import drivers_inc


src_dir = Path('../src/')
constants_path = src_dir.joinpath('constants.hpp')
drivers_inc_dir_path = src_dir.joinpath('drivers/inc')
computer_hpp_dir_path = src_dir.joinpath('computers/inc')
computer_cpp_dir_path = src_dir.joinpath('computers/src')

template_dir = Path('./templates/')
computer_cpp_template_path = template_dir.joinpath('computer.cpp.j2')
computer_hpp_template_path = template_dir.joinpath('computer.hpp.j2')
constants_template_path = template_dir.joinpath('constants.hpp.j2')
drivers_inc_template_dir_path = template_dir.joinpath('drivers/inc')

from pint import UnitRegistry
def get_ms(period_str):
    if type(period_str) is int:
        # If it's set as an integer, assume ms
        return period_str

    ur = UnitRegistry()
    return (int)(ur[period_str].to('ms').magnitude)

def get_msg_len(msg):
    if hasattr(msg, "frame"):
       return 1 + sum([get_msg_len(sub_frame) for sub_frame in msg.frame])
    return 1

# FROM: https://github.com/duelafn/python-jinja2-apci/blob/master/jinja2_apci/error.py
class RaiseExtension(Extension):
    # This is our keyword(s):
    tags = set(['raise'])

    # See also: jinja2.parser.parse_include()
    def parse(self, parser):
        # the first token is the token that started the tag. In our case we
        # only listen to "raise" so this will be a name token with
        # "raise" as value. We get the line number so that we can give
        # that line number to the nodes we insert.
        lineno = next(parser.stream).lineno

        # Extract the message from the template
        message_node = parser.parse_expression()

        return nodes.CallBlock(
            self.call_method('_raise', [message_node], lineno=lineno),
            [], [], [], lineno=lineno
        )

    def _raise(self, msg, caller):
        raise TemplateRuntimeError(msg)


def render_template_from_to(env, input_path, output_path):
    template = env.get_template(str(input_path))
    with open(output_path, 'w') as f:
        if output_path == src_dir.joinpath("structs.hpp"):
            f.write(template.render(get_ms = get_ms, get_msg_len = get_msg_len))
        else:
            f.write(template.render())


def render_template(env, relative_path):
    render_template_from_to(env, template_dir.joinpath(f"{relative_path}.j2"), src_dir.joinpath(relative_path))



if __name__ == '__main__':
    specpath = sys.argv[1]
    specfile = open(specpath, 'r')
    system = ParseCAN.spec.System.from_yaml(specfile)
    can = system.protocol['name']['can']

    script_dir = os.path.dirname(sys.argv[0])
    if script_dir == '':
        script_dir = '.'
    os.chdir(script_dir)

    template_loader = jinja2.FileSystemLoader(searchpath=".")
    template_env = jinja2.Environment(loader=template_loader, keep_trailing_newline=True, extensions=[RaiseExtension])
    template_env.globals["can"] = can
    template_env.globals["system"] = system

    for filename in ["pack_unpack.cpp", "structs.hpp", "bus.hpp", "bus.cpp", "structs.cpp"]:
        render_template(template_env, filename)

#    constants.write(template_env, constants_template_path, constants_path)
    computers_hpp.write(template_env, system.computer, computer_hpp_template_path, computer_hpp_dir_path)
    computers_cpp.write(template_env, system.computer, computer_cpp_template_path, computer_cpp_dir_path)
    drivers_inc.write(template_env, system, drivers_inc_template_dir_path, drivers_inc_dir_path)
    import os
    os.system('clang-format -i ../src/structs.hpp ../src/bus.cpp ../src/bus.hpp ../src/pack_unpack.cpp ../src/structs.cpp')
    os.system('clang-format -i ../src/drivers/inc/* ../src/drivers/src/*')
