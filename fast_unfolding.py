#!/usr/bin/env python3
import argparse


NULLDEV='/dev/null'


parser = argparse.ArgumentParser(description='Perform the fast unfolding algorithm over a bivariate distribution.')


parser.add_argument('input_file', metavar='INPUT', type=str,
                    help='the input file that contains the bivariate distribution')
                    
parser.add_argument('vcount', metavar='N', type=int,
                    help='the number of vertices in the graph')
                    
parser.add_argument('output_file', metavar='OUTPUT', type=str,
                    help='the output file to store the detected communities')

parser.add_argument('-t', '--time', metavar='FILE', dest='time_file', type=str, default=NULLDEV,
                    help='record the consumption time to FILE')

parser.add_argument('-s', '--size', metavar='FILE', dest='size_file', type=str, default=NULLDEV,
                    help='record the number of vertices in each round to FILE')

parser.add_argument('-i', '--iter', metavar='FILE', dest='iter_file', type=str, default=NULLDEV,
                    help='record the number of iterations in each round to FILE')
                    
parser.add_argument('-3c', '--three_column', dest='three_column', action='store_true', default=False,
                    help='use three-column formation for bivariate distribution')

args = parser.parse_args()

target_execute_file = './fastunfolding_3c' if args.three_column else './fastunfolding'

cmd = '{} {} {} {} {} {} > {}'.format(target_execute_file, args.input_file, args.vcount, args.time_file, args.size_file, args.iter_file, args.output_file)

import subprocess
# subprocess.Popen(cmd, shell=True)
print(cmd)
