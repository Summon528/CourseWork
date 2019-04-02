#!/usr/local/bin/python3

import argparse
import yaml
import subprocess
import schedule
import time
import os
import sys
import signal

ZBACKUP_PATH = "/usr/local/bin/zbackup"
LOG_PATH = '/var/log/zbackup.log'

parser = argparse.ArgumentParser()
parser.add_argument('-c', '--config', help='path to config', dest='config',
                    default='/usr/local/etc/zbackupd.yaml')
parser.add_argument('-p', '--pid', help='path to pid file', dest='pid_file',
                    default='/var/run/zbackup.pid')
parser.add_argument('-d', action='store_true', dest='daemon')
args = parser.parse_args()

time_length = {
    's': 1,
    'm': 60,
    'h': 60 * 60,
    'd': 60 * 60 * 24,
    'w': 60 * 60 * 24 * 7,
}


def handle_term(signum, frame):
    os.remove(args.pid_file)
    sys.exit(0)


signal.signal(signal.SIGTERM, handle_term)


def parse_interval(interval):
    striped = interval.strip()
    return int(striped[:-1]) * time_length[striped[-1]]


def start_job(dataset):
    with open(LOG_PATH, 'a+') as f:
        subprocess.call([
            ZBACKUP_PATH,
            dataset['dataset'],
            str(dataset['rotation'])
        ], stdout=f)
    time.sleep(0.1)


def start_schedule(signum=None, frame=None):
    schedule.clear('job')
    with open(args.config, 'r') as f:
        config = yaml.load(f.read())
    for dataset in config['backup']:
        if 'enabled' not in dataset or dataset['enabled']:
            schedule.every(parse_interval(
                dataset['period'])).seconds.do(start_job, dataset=dataset).tag('job')
    if signum == None:
        schedule.run_all()
    while True:
        schedule.run_pending()
        time.sleep(1)


signal.signal(signal.SIGUSR1, start_schedule)


def main():
    if os.path.isfile(args.pid_file):
        print("Daemon already runing", file=sys.stderr)
        sys.exit(1)
    if args.daemon:
        pid = os.fork()
        if pid > 0:
            with open(args.pid_file, 'w') as f:
                f.write(str(pid))
            sys.exit(0)
        start_schedule()
    else:
        start_schedule()


if __name__ == '__main__':
    main()
