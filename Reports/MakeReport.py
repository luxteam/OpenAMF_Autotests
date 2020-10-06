from jinja2 import Environment, FileSystemLoader, select_autoescape
import sys
import json
import os
import argparse
from glob import glob
from shutil import copytree, rmtree
import logging


logging.basicConfig(filename='report_building.log', level=logging.INFO, format='%(asctime)s :: %(levelname)s :: %(message)s')
logger = logging.getLogger(__name__)


def main():
    logger.info('Started report building')
    rc = 0

    parser = argparse.ArgumentParser()
    parser.add_argument('--commit_hash', required=True)
    parser.add_argument('--branch_name', required=True)
    parser.add_argument('--test_results', required=True)
    args = parser.parse_args()

    try:
        if os.path.exists(os.path.join(args.test_results, 'resources')):
            rmtree(os.path.join(args.test_results, 'resources'), True)
        copytree('resources', os.path.join(args.test_results, 'resources'))
        logger.info('Report resources were copied successfully')
    except Exception as err:
        logger.error('Fail during copying report resources: {}'.format(str(err)))

    # Reading from all .json files with results from all GPUs (gotten from directory specified by test_results param)
    statistics = list()
    output_json_files = glob(os.path.join(args.test_results, '*.json'))
    for output_json_file in output_json_files:
        with open(output_json_file, 'r') as file:
            statistics.append(json.load(file))

    env = Environment(
        loader=FileSystemLoader('templates')
    )

    ### Creating main page ###    
    template = env.get_template('main_page.html')

    # Forming information for 'Summary' section
    gpu_stat = list()
    for result in statistics:
        gpu_stat.append(
            {
            'Platform' : result['platform'],
            'Configuration' : result['configuration'],
            'Hostname' : result['hostname'],
            'Total' : result['tests'],
            'Passed' : result['tests'] - result['failures'] - result['disabled'] - result['errors'],
            'Failed' : result['failures'],
            'Disabled' : result['disabled'],
            'Error' : result['errors'],
            'Time taken': result['time'],
            'Test suites': result['testsuites']
            }
        )

    try:
        out_file = template.render(
            gpu_stat=gpu_stat,
            commit_hash=args.commit_hash,
            branch_name=args.branch_name
        )
    except Exception as err:
        logger.error('Fail during summary report building: {}'.format(str(err)))
        rc = -1

    with open(os.path.join(args.test_results, 'mainPage.html'), 'w') as fh:
        fh.write(out_file)


    # Forming information for 'Details' sections
    template = env.get_template('test_suite_details_page.html')
    for stat in statistics:
        for result in stat['testsuites']:

            for test_case in result['testsuite']:
                if 'DISABLED_' in test_case['name']:
                    test_case['Result'] = 'Skipped'
                elif not 'failures' in test_case:
                    test_case['Result'] = 'Passed'
                else:
                    test_case['Result'] = 'Failure<br>'
                    for fail in test_case['failures']:
                        test_case['Result'] += fail['failure']

            test_suite = {
                'Platform' : stat['platform'],
                'Configuration' : stat['configuration'],
                'Name' : result["name"],
                'Total' : result['tests'],
                'Passed' : result['tests'] - result['failures'] - result['disabled'] - result['errors'],
                'Failed' : result['failures'],
                'Disabled' : result['disabled'],
                'Error' : result['errors'],
                'Time taken': result['time'],
                'Time stamp': result['timestamp'].replace('T', ' ').replace('Z', ''),
                'Test cases': result['testsuite']
                }
            
            detailed_report_name = 'details_' + result['name'] + '_' + stat['platform'] + '_' + stat['configuration'] + '.html'
            try:
                out_file = template.render(test_suite=test_suite)
            except Exception as err:
                logger.error('Fail during "{}" report building: {}'.format(detailed_report_name, str(err)))
                rc = -1

            with open(os.path.join(args.test_results, detailed_report_name), 'w') as fh:
                fh.write(out_file)

    return rc


if __name__ == "__main__":
    exit(main())
