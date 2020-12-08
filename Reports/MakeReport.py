from jinja2 import Environment, FileSystemLoader, select_autoescape
import sys
import json
import os
import argparse
from glob import glob
from shutil import copytree, rmtree
import logging
import re
from datetime import datetime

SKIP_FILE = os.path.join('resources', 'tests_skips.json')


logging.basicConfig(filename='report_building.log', level=logging.INFO, format='%(asctime)s :: %(levelname)s :: %(message)s')
logger = logging.getLogger(__name__)

def find_code(case_name):
    for source_file in glob(os.path.join("../src", '*.cpp')):
        with open(source_file, 'r', encoding='utf-8') as source_code:
            file_text = source_code.read()

        nested_pattern = r"({[^{}]*(?:({[^{}]*(?:({[^{}]*(?:({[^{}]*(?:({[^{}]*(?:({[^{}]*(?:({[^{}]*(?:({[^{}]*(?:({[^{}]*(?:({[^{}]*(?:({[^{}]*?})+?[^{}]*)*?})+?[^{}]*)*?})+?[^{}]*)*?})+?[^{}]*)*?})+?[^{}]*)*?})+?[^{}]*)*?})+?[^{}]*)*?})+?[^{}]*)*?})+?[^{}]*)*?})+?[^{}]*)*?})"
        try:
            return re.search(r"%s\)\s*%s" % (case_name, nested_pattern), file_text).group(1)
            # return re.search(r"%s\)\s*{((.|\n)*?(?=}))" % case_name, file_text).group(1)
        except Exception as e:
            pass

def calculateNotImplemented(stat):
    total_non_implemented = 0
    for result in stat['testsuites']:
        for test_case in result['testsuite']:
            if 'failures' in test_case and any('actual: 9 vs 9' in fail['failure'] for fail in test_case['failures']):
                total_non_implemented += 1
    return total_non_implemented

def calculateTestSuiteNotImplemented(suite):
    total_non_implemented = 0
    for test_case in suite:
        if 'failures' in test_case and any('actual: 9 vs 9' in fail['failure'] for fail in test_case['failures']):
            total_non_implemented += 1
    return total_non_implemented

def main():
    logger.info('Started report building')
    rc = 0

    parser = argparse.ArgumentParser()
    parser.add_argument('--commit_hash', required=True)
    parser.add_argument('--branch_name', required=True)
    parser.add_argument('--commit_message', required=True)
    parser.add_argument('--commit_datetime', required=True)
    parser.add_argument('--test_results', required=True)
    args = parser.parse_args()

    try:
        # if os.path.exists(os.path.join(args.test_results, 'resources')):
        #     rmtree(os.path.join(args.test_results, 'resources'), True)
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
        for res in result['testsuites']:
            res['not_implemented'] = calculateTestSuiteNotImplemented(res['testsuite'])
            res['failures'] -= res['not_implemented']
        not_implemented = sum(map(calculateNotImplemented, statistics))
        gpu_stat.append(
            {
            'Platform' : result['platform'],
            'Configuration' : result['configuration'],
            'Hostname' : result['hostname'],
            'Total' : result['tests'],
            'Passed' : result['tests'] - result['failures'] - result['disabled'] - result['errors'],
            'Failed' : result['failures'] - not_implemented,
            'Disabled' : result['disabled'],
            'Error' : result['errors'],
            'Not implemented' : not_implemented,
            'Time taken': result['time'],
            'Test suites': sorted(result['testsuites'], key=lambda suite : suite['name'])
            }
        )

    # parse timezone
    commit_datetime_object = datetime_object = datetime.strptime(args.commit_datetime, "%Y-%m-%d %H:%M:%S %z")
    commit_datetime = commit_datetime_object.strftime("%Y-%m-%d %H:%M:%S")

    try:
        out_file = template.render(
            gpu_stat=gpu_stat,
            commit_hash=args.commit_hash,
            branch_name=args.branch_name,
            commit_message=args.commit_message.encode().decode('unicode-escape'),
            commit_datetime=commit_datetime
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
                # Fix timestamps
                if (int(test_case['timestamp'].replace('T', ' ').replace('Z', '')[:4]) < 2000):
                    test_case['timestamp'] = '-'

                # Fetch test cases's code from source files
                test_case['Code'] = find_code(test_case['name'])
                if (test_case['Code']):
                    test_case['Code'] = test_case['Code'].replace('\n', '<br>')

                # Process test's statuses
                test_case['Fails Reason'] = ''
                if 'DISABLED_' in test_case['name']:
                    test_case['Result'] = 'Skipped'
                    try:
                        with open(SKIP_FILE, 'r') as file:
                            skips_reasons = json.load(file)
                            if test_case['name'] in skips_reasons:
                                test_case['Result'] += ' due to <br><a href = %s>%s</a>' % (skips_reasons[test_case['name']], skips_reasons[test_case['name']])
                    except Exception as e:
                        print(e.message)
                elif not 'failures' in test_case:
                    test_case['Result'] = 'Passed'
                elif any('actual: 9 vs 9' in fail['failure'] for fail in test_case['failures']):
                    test_case['Result'] = 'Not implemented'
                else:
                    test_case['Result'] = 'Failure<br>'
                    for fail in test_case['failures']:
                        test_case['Fails Reason'] += fail['failure']

            test_suite = {
                'Platform' : stat['platform'],
                'Configuration' : stat['configuration'],
                'Name' : result["name"],
                'Total' : result['tests'],
                'Passed' : result['tests'] - result['failures'] - result['disabled'] - result['errors'],
                'Failed' : result['failures'],
                'Disabled' : result['disabled'],
                'Error' : result['errors'],
                'Not implemented' : calculateNotImplemented(stat),
                'Time taken': result['time'],
                'Time stamp': result['timestamp'].replace('T', ' ').replace('Z', ''),
                'Test cases': [v for v in sorted (result['testsuite'], key=lambda item: item['Result'])]
                }
            
            detailed_report_name = 'details_' + result['name'] + '_' + stat['platform'] + '_' + stat['configuration'] + '.html'
            try:
                out_file = template.render(test_suite=test_suite)
            except Exception as err:
                logger.error('Fail during "{}" report building: {}'.format(detailed_report_name, str(err)))
                rc = -1

            with open(os.path.join(args.test_results, detailed_report_name), 'w', encoding='utf-8') as fh:
                fh.write(out_file)

    return rc


if __name__ == "__main__":
    exit(main())
