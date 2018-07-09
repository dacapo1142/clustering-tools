function [ collection, objective ] = ksetsplus(g, n, k, varargin)
pnames={'measure','verbose', 'which_cluster'};
dflts={'distance',false,[]};
[measure, verbose, which_cluster]=internal.stats.parseArgs(pnames, dflts, varargin{:});

addpath('containers');
if which_cluster
    collection=DisjointSets(n,k,which_cluster);
else
    collection=DisjointSets(n,k);
end
iter_order=1:n;
changed=true;
count=0;

if verbose
    tic;
    fprintf('------------------------------------------------------------------------------------\n');
    fprintf('The function is going to display the changing of the objective value in each run.\n');
    switch measure
        case 'distance'
            objective=0;
            for cid=1:k
                cluster=collection.cluster(cid);
                csize=collection.csize(cid);
                gss=g(cluster,cluster);
                objective=objective+sum(gss(:))/csize;
            end
    end
    fprintf('The original objective value is: %f\n', objective);
    fprintf('------------------------------------------------------------------------------------\n');
end

while changed
    
    count=count+1;
    
    changed=false;
    for vid=iter_order
        min_adjust_distance=inf;
        old_cid=collection.which_cluster(vid);
        if collection.csize(old_cid) ==1
            continue;
        end
        
        for cid=1:k
            cluster=collection.cluster(cid);
            cluster_size=collection.csize(cid);
            switch measure
                case 'cohesion'
                    delta_distance=g(vid,vid)...
                        -2/cluster_size*sum(g(vid, cluster))...
                        +1/cluster_size^2*sum(sum(g(cluster,cluster)));
                case 'distance'
                    delta_distance=...
                        (2*sum(g(vid,cluster)/cluster_size)...
                        -sum(sum(g(cluster,cluster)))/cluster_size^2);
            end
            
            if cid==old_cid
                adjust_distance=delta_distance*cluster_size/(cluster_size-1);
            else
                adjust_distance=delta_distance*cluster_size/(cluster_size+1);
            end
            
            if adjust_distance<min_adjust_distance
                min_adjust_distance=adjust_distance;
                best_cid=cid;
            end
        end
        
        if best_cid ~= old_cid
            collection.move(vid, best_cid);
            changed=true;
        end
    end
    if nargout>1
        switch measure
            case 'distance'
                objective=0;
                for cid=1:k
                    cluster=collection.cluster(cid);
                    csize=collection.csize(cid);
                    gss=g(cluster,cluster);
                    objective=objective+sum(gss(:))/csize;
                end
        end
    end
    if verbose
        fprintf('%f\n',objective);
    end
end

if verbose
    fprintf('------------------------------------------------------------------------------------\n');
    time_consumption=toc;
    fprintf('The number of iterations: %d\n', count);
    fprintf('Time consumption: %f seconds\n', time_consumption);
end
