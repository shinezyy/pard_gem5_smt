from matplotlib import pyplot as plt
import pandas as pd


def read_raw(filename, sep):
    data_frame_raw = pd.read_csv(
        filepath_or_buffer=filename, header=0, sep=sep,
        engine='python'
    )
    return data_frame_raw


def proc(filename, target_stat, csv_len):
    my_dpi = 100

    st_utils = [
        # u' system.cpu.iew.lsq.lq_utilization ',
        u' system.cpu.iew.lsq.sq_utilization ',
        # u' system.cpu.iq.iq_utilization ',
        # u' system.cpu.rename.intPhyReg_utilization ',
        # u' system.cpu.rob.rob_utilization '
    ]

    smt_utils = [
        u' system.cpu.iew.lsq.lq_utilization::0 ',
        u' system.cpu.iew.lsq.lq_utilization::total ',
        # u' system.cpu.iew.lsq.sq_utilization::0 ',
        # u' system.cpu.iew.lsq.sq_utilization::total ',
        # u' system.cpu.iq.iq_utilization::0 ',
        # u' system.cpu.iq.iq_utilization::total ',
    ]

    ipcs_st = [
        u' system.cpu.ipc_total ',
        # u' local_ipc'
    ]
    ipcs_smt = [
        u' system.cpu.ipc_total ',
        u' system.cpu.ipc::0 ',
        # u' local_ipc_0'
    ]

    miss_rate = [
        u' system.cpu.dcache.ReadReq_miss_rate::total ',
        u' system.cpu.dcache.WriteReq_miss_rate::total ',
        u' system.cpu.icache.ReadReq_miss_rate::total ',
    ]

    td = dict()
    td['st_util'] = st_utils
    td['smt_util'] = smt_utils
    td['st_ipc'] = ipcs_st
    td['smt_ipc'] = ipcs_smt
    td['smt_cache'] = miss_rate

    print 'Processing', filename.split('.')[0]+'_'+target_stat, '...'
    targets = td[target_stat]

    csv = read_raw(filename, ',')
    print csv.columns

    x_axis = csv[u' system.cpu.numCycles '].values[0:csv_len]
    # x_axis = csv[u' system.cpu.rob.rob_utilization '].values[0:csv_len]
    # print 'cycles: ', x_axis

    plt.figure(figsize=(2560/my_dpi, 1600/my_dpi), dpi=my_dpi)

    for h in targets:
        y_axis = csv[h].values[0:csv_len]
        plt.plot(x_axis, y_axis)

    plt.legend(targets, loc='upper left')
    print 'Output file :', filename.split('.')[0]+'_'+target_stat+'.png'
    plt.savefig(filename.split('.')[0]+'_'+target_stat+'.png', dpi=my_dpi)
    # plt.show()


def main():
    smt_stats = [
        'bzip2_perl.csv',
        'perl_bzip2.csv',
        'perl_gcc.csv',
        'gcc_perl.csv',
    ]

    st_stats = [
        'bzip2.csv',
        'perl.csv',
        'gcc.csv',
    ]

    cache_stats = ['perl_mcf.csv', 'perl_mcf_dyn.csv']

    for benchmark in cache_stats:
        proc(benchmark, 'smt_ipc', 65000)


if __name__ == '__main__':
    main()